/**
 * @file can_network_service.c
 * @brief CAN 网络服务层核心实现 (纯 C 语言)
 * * 本文件封装了 ESP32-S3 的 TWAI (Two-Wire Automotive Interface) 驱动。
 * 实现了 CAN 总线的硬件初始化、报文的非阻塞轮询接收、协议解包，
 * 并自动将解析后的物理数据同步到全局的 SensorState 中。
 */

#include "can_network_service.h"
#include "sensor_state.h"       // 引入数字孪生状态层，用于数据写入
#include <string.h>

// FreeRTOS 与 ESP-IDF 头文件
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char* TAG = "CAN_SVC";

// ============================================================================
// 私有静态状态与资源 (对外部严格隐藏)
// ============================================================================

static bool s_is_initialized = false;
static bool s_is_started = false;
static bool s_recovery_in_progress = false;
static CanConfig s_config;
static CanMetrics s_metrics = {0};

/** 用于保护状态和统计指标的互斥锁 */
static SemaphoreHandle_t s_mutex = NULL;

// ============================================================================
// 私有辅助函数
// ============================================================================

/**
 * @brief 根据波特率获取对应的 TWAI 时序配置
 */
static twai_timing_config_t get_timing_config(uint32_t baud_rate) {
    switch (baud_rate) {
        case 1000000: return (twai_timing_config_t)TWAI_TIMING_CONFIG_1MBITS();
        case 800000:  return (twai_timing_config_t)TWAI_TIMING_CONFIG_800KBITS();
        case 500000:  return (twai_timing_config_t)TWAI_TIMING_CONFIG_500KBITS();
        case 250000:  return (twai_timing_config_t)TWAI_TIMING_CONFIG_250KBITS();
        case 125000:  return (twai_timing_config_t)TWAI_TIMING_CONFIG_125KBITS();
        case 100000:  return (twai_timing_config_t)TWAI_TIMING_CONFIG_100KBITS();
        case 50000:   return (twai_timing_config_t)TWAI_TIMING_CONFIG_50KBITS();
        case 25000:   return (twai_timing_config_t)TWAI_TIMING_CONFIG_25KBITS();
        default:
            ESP_LOGW(TAG, "未知的波特率 %lu bps, 默认回退至 250kbps", baud_rate);
            return (twai_timing_config_t)TWAI_TIMING_CONFIG_250KBITS();
    }
}

/**
 * @brief 尝试恢复处于 Bus-Off 状态的 CAN 总线
 */
static void attempt_bus_recovery(twai_state_t state) {
    if (state == TWAI_STATE_BUS_OFF) {
        if (!s_recovery_in_progress) {
            ESP_LOGW(TAG, "检测到 BUS_OFF，开始恢复流程...");
            esp_err_t err = twai_initiate_recovery();
            if (err == ESP_OK) {
                s_recovery_in_progress = true;
                xSemaphoreTake(s_mutex, portMAX_DELAY);
                s_is_started = false;
                xSemaphoreGive(s_mutex);
                ESP_LOGI(TAG, "CAN 总线进入恢复中状态");
            } else {
                ESP_LOGE(TAG, "CAN 总线恢复失败: 0x%X", err);
            }
        }
        return;
    }

    if (state == TWAI_STATE_STOPPED && s_recovery_in_progress) {
        esp_err_t err = twai_start();
        if (err == ESP_OK) {
            xSemaphoreTake(s_mutex, portMAX_DELAY);
            s_is_started = true;
            xSemaphoreGive(s_mutex);
            s_recovery_in_progress = false;
            ESP_LOGI(TAG, "CAN 总线恢复成功");
        } else {
            ESP_LOGE(TAG, "CAN 总线恢复失败: 0x%X", err);
        }
    }
}

/**
 * @brief 仅判断驱动是否安装完成（用于 poll 任务保持运行）
 */
static bool can_service_driver_ready(void) {
    bool init_state = false;
    if (s_mutex) {
        xSemaphoreTake(s_mutex, portMAX_DELAY);
        init_state = s_is_initialized;
        xSemaphoreGive(s_mutex);
    }
    return init_state;
}

/**
 * @brief 处理单条接收到的 CAN 报文，并将其路由到正确的业务逻辑中
 */
static void process_received_message(const twai_message_t* msg) {
    // 过滤：仅处理标准帧，忽略扩展帧或远程帧
    if (msg->extd || msg->rtr) {
        return; 
    }
    
    // 过滤：协议严格要求 8 字节定长
    if (msg->data_length_code != CAN_DATA_SIZE) {
        ESP_LOGD(TAG, "忽略非标准长度报文: DLC=%d", msg->data_length_code);
        return;
    }
    
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    s_metrics.rx_count++;
    xSemaphoreGive(s_mutex);
    
    // 调用 can_protocol 层进行安全解析
    CanParsedPacket packet;
    if (!can_proto_parse_packet(msg->identifier, msg->data, &packet)) {
        xSemaphoreTake(s_mutex, portMAX_DELAY);
        s_metrics.parse_error_count++;
        xSemaphoreGive(s_mutex);
        ESP_LOGD(TAG, "协议解析失败, ID: 0x%03X", msg->identifier);
        return;
    }
    
    // 解析成功，将数据同步至 SensorState (数字孪生层)
    if (packet.is_valid) {
        // 更新在线状态 (心跳刷新)
        sensor_state_mark_online(packet.node_id);
        
        switch (packet.func_code) {
            case CAN_FUNC_REPORT:
                // 从机上报当前物理读数
                sensor_state_update_current(packet.node_id, (uint8_t)packet.param_index, packet.scaled_value);
                break;
                
            case CAN_FUNC_WRITE_SET:
                // 主机下发设定的回采确认 (ACK)
                sensor_state_update_target(packet.node_id, (uint8_t)packet.param_index, packet.scaled_value);
                break;
                
            case CAN_FUNC_ALERT:
                // 告警报文处理 (可扩展回调机制)
                ESP_LOGW(TAG, "节点 %d 触发告警! 参数: %s, 值: %.2f", 
                         packet.node_id, can_proto_get_param_name(packet.param_index), packet.scaled_value);
                break;
                
            case CAN_FUNC_TIME_SYNC:
                // 时间同步处理逻辑
                break;
                
            default:
                break;
        }
    }
}

// ============================================================================
// 生命周期管理 API 实现
// ============================================================================

bool can_service_init(const CanConfig* config) {
    if (config == NULL) {
        return false;
    }

    if (s_mutex == NULL) {
        s_mutex = xSemaphoreCreateMutex();
        if (s_mutex == NULL) return false;
    }
    
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    if (s_is_initialized) {
        xSemaphoreGive(s_mutex);
        ESP_LOGW(TAG, "CAN 服务已初始化，忽略重复调用");
        return true;
    }
    s_config = *config;
    xSemaphoreGive(s_mutex);

    ESP_LOGI(TAG, "正在初始化 CAN(TWAI) 驱动... TX:GPIO%d, RX:GPIO%d, 波特率:%lubps", 
             config->tx_pin, config->rx_pin, config->baud_rate);

    // 1. 常规配置
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(config->tx_pin, config->rx_pin, config->mode);
    g_config.rx_queue_len = config->queue_size;
    g_config.tx_queue_len = config->queue_size / 2 > 0 ? config->queue_size / 2 : 5;
    
    // 2. 时序配置
    twai_timing_config_t t_config = get_timing_config(config->baud_rate);
    
    // 3. 过滤器配置 (接收所有标准帧)
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // 安装驱动
    esp_err_t err = twai_driver_install(&g_config, &t_config, &f_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "TWAI 驱动安装失败: 0x%X", err);
        return false;
    }

    // 启动驱动
    err = twai_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "TWAI 驱动启动失败: 0x%X", err);
        twai_driver_uninstall();
        return false;
    }

    xSemaphoreTake(s_mutex, portMAX_DELAY);
    s_is_initialized = true;
    s_is_started = true;
    memset(&s_metrics, 0, sizeof(CanMetrics));
    xSemaphoreGive(s_mutex);

    ESP_LOGI(TAG, "CAN 网络服务启动成功");
    return true;
}

void can_service_stop(void) {
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    if (!s_is_initialized) {
        xSemaphoreGive(s_mutex);
        return;
    }
    
    if (s_is_started) {
        twai_stop();
        s_is_started = false;
    }
    twai_driver_uninstall();
    s_is_initialized = false;
    xSemaphoreGive(s_mutex);
    
    ESP_LOGI(TAG, "CAN 网络服务已停止并卸载");
}

bool can_service_is_initialized(void) {
    bool init_state = false;
    if (s_mutex) {
        xSemaphoreTake(s_mutex, portMAX_DELAY);
        init_state = s_is_initialized && s_is_started;
        xSemaphoreGive(s_mutex);
    }
    return init_state;
}

// ============================================================================
// 核心调度 API 实现
// ============================================================================

void can_service_poll(void) {
    if (!can_service_driver_ready()) {
        return;
    }

    // 周期性检查硬件总线状态与异常
    twai_status_info_t status;
    if (twai_get_status_info(&status) == ESP_OK) {
        if (status.state == TWAI_STATE_RUNNING) {
            twai_message_t msg;
            // 非阻塞轮询接收队列。如果队列中有数据，则快速处理完。
            while (twai_receive(&msg, 0) == ESP_OK) {
                process_received_message(&msg);
            }
        }

        if (status.state == TWAI_STATE_BUS_OFF) {
            xSemaphoreTake(s_mutex, portMAX_DELAY);
            s_metrics.bus_error_count++;
            xSemaphoreGive(s_mutex);

            attempt_bus_recovery(status.state);
        } else if (status.state == TWAI_STATE_STOPPED || status.state == TWAI_STATE_RECOVERING) {
            attempt_bus_recovery(status.state);
        }
    }
}

// ============================================================================
// 数据发送 API 实现
// ============================================================================

bool can_service_send_control(uint8_t node_id, CanParamIndex param_index, float physical_value) {
    if (!can_service_is_initialized()) {
        ESP_LOGE(TAG, "无法发送控制指令: CAN 服务未运行");
        return false;
    }

    if (node_id > MAX_NODE_ID) {
        ESP_LOGE(TAG, "无法发送: 节点 ID %d 越界", node_id);
        return false;
    }

    twai_message_t tx_msg;
    tx_msg.extd = 0;              // 标准帧
    tx_msg.rtr = 0;               // 数据帧
    tx_msg.data_length_code = 8;  // 固定 8 字节载荷

    // 构建 CAN ID
    tx_msg.identifier = can_proto_build_id(CAN_FUNC_WRITE_SET, node_id);

    // 构建数据载荷
    can_proto_build_packet(tx_msg.data, param_index, physical_value);

    // 尝试发送，超时时间设定为 50ms (防止 UI 线程长时间阻塞)
    esp_err_t err = twai_transmit(&tx_msg, pdMS_TO_TICKS(50));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "发送控制指令失败 (ID:0x%03X), 错误码: 0x%X", tx_msg.identifier, err);
        return false;
    }

    xSemaphoreTake(s_mutex, portMAX_DELAY);
    s_metrics.tx_count++;
    xSemaphoreGive(s_mutex);

    ESP_LOGD(TAG, "发送控制指令 -> 节点:%d, 参数:%s, 设定值:%.2f", 
             node_id, can_proto_get_param_name(param_index), physical_value);
    return true;
}

bool can_service_send_raw(uint16_t can_id, const uint8_t* data, size_t data_length) {
    if (!can_service_is_initialized() || data == NULL || data_length != 8) {
        return false;
    }

    twai_message_t tx_msg;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = data_length;
    tx_msg.identifier = can_id & 0x7FF; // 确保是 11-bit ID
    memcpy(tx_msg.data, data, data_length);

    esp_err_t err = twai_transmit(&tx_msg, pdMS_TO_TICKS(50));
    if (err == ESP_OK) {
        xSemaphoreTake(s_mutex, portMAX_DELAY);
        s_metrics.tx_count++;
        xSemaphoreGive(s_mutex);
        return true;
    }
    return false;
}

// ============================================================================
// 诊断与状态监控 API 实现
// ============================================================================

void can_service_get_metrics(CanMetrics* out_metrics) {
    if (out_metrics && s_mutex) {
        xSemaphoreTake(s_mutex, portMAX_DELAY);
        *out_metrics = s_metrics;
        xSemaphoreGive(s_mutex);
    }
}

bool can_service_get_twai_status(twai_status_info_t* out_status) {
    if (!can_service_is_initialized() || out_status == NULL) {
        return false;
    }
    return twai_get_status_info(out_status) == ESP_OK;
}

void can_service_reset_metrics(void) {
    if (s_mutex) {
        xSemaphoreTake(s_mutex, portMAX_DELAY);
        memset(&s_metrics, 0, sizeof(CanMetrics));
        xSemaphoreGive(s_mutex);
        ESP_LOGI(TAG, "CAN 统计指标已清零");
    }
}

void can_service_print_diagnostics(void) {
    ESP_LOGI(TAG, "=== CAN 网络服务诊断报告 ===");
    
    bool is_init = false;
    if (s_mutex) {
        xSemaphoreTake(s_mutex, portMAX_DELAY);
        is_init = s_is_initialized;
        ESP_LOGI(TAG, "状态: %s", (s_is_initialized && s_is_started) ? "RUNNING" : "STOPPED");
        if (s_is_initialized) {
            ESP_LOGI(TAG, "应用层统计 -> 接收:%lu, 发送:%lu, 解析错误:%lu, 硬件异常:%lu",
                     s_metrics.rx_count, s_metrics.tx_count, 
                     s_metrics.parse_error_count, s_metrics.bus_error_count);
        }
        xSemaphoreGive(s_mutex);
    }

    if (is_init) {
        twai_status_info_t status;
        if (twai_get_status_info(&status) == ESP_OK) {
            const char* hw_state = "UNKNOWN";
            switch(status.state) {
                case TWAI_STATE_RUNNING: hw_state = "RUNNING"; break;
                case TWAI_STATE_STOPPED: hw_state = "STOPPED"; break;
                case TWAI_STATE_BUS_OFF: hw_state = "BUS_OFF"; break;
                case TWAI_STATE_RECOVERING: hw_state = "RECOVERING"; break;
            }
            ESP_LOGI(TAG, "硬件底状态 -> %s", hw_state);
            ESP_LOGI(TAG, "底层收发队列 -> 待读取(RX):%lu, 待发送(TX):%lu", 
                     status.msgs_to_rx, status.msgs_to_tx);
            ESP_LOGI(TAG, "硬件总线错误计数 -> TX_Err:%lu, RX_Err:%lu", 
                     status.tx_error_counter, status.rx_error_counter);
        }
    }
    ESP_LOGI(TAG, "============================");
}