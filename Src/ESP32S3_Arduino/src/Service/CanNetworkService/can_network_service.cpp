/**
 * @file can_network_service.cpp
 * @brief CAN 网络服务层实现文件
 * * 此文件实现 CanNetworkService 类的所有方法，提供完整的 CAN 网络服务功能。
 * 包括硬件初始化、消息收发、协议解析、错误处理和数据同步。
 */

#include "can_network_service.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "../BSP/Os_Service/os_service.h" // 假设路径正确
#include "global_config.h"               // 假设路径正确

// 本地日志标签
static const char* TAG = "CanNetworkService";

namespace Service {

// ============================================================================
// 构造函数与析构函数实现
// ============================================================================

CanNetworkService::CanNetworkService(Greenhouse::SensorState& sensor_state)
    : is_initialized_(false)
    , is_started_(false)
    , sensor_state_(sensor_state)
    , rx_count_(0)
    , tx_count_(0)
    , parse_error_count_(0)
    , bus_error_count_(0)
{
    debugPrint("[CAN] CanNetworkService object created with dependency injected SensorState");
}

CanNetworkService::~CanNetworkService()
{
    stop();
    debugPrint("[CAN] CanNetworkService object destroyed");
}

// ========================================================================
// 私有方法实现
// ========================================================================

twai_general_config_t CanNetworkService::prepareGeneralConfig(const CanConfig& config) const
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        config.tx_pin, 
        config.rx_pin, 
        config.mode
    );
    g_config.rx_queue_len = config.queue_size;
    g_config.tx_queue_len = config.queue_size / 2;  // 发送队列稍小
    g_config.rx_io = config.rx_pin;
    g_config.tx_io = config.tx_pin;
    return g_config;
}

twai_timing_config_t CanNetworkService::prepareTimingConfig(uint32_t baud_rate) const
{
    // 根据波特率选择合适的时序配置
    // 使用标准 CAN 2.0 时序，采样点为 87.5%
    switch (baud_rate) {
        case 1000000:   // 1 Mbps
            return TWAI_TIMING_CONFIG_1MBITS();
        case 800000:    // 800 kbps
            return TWAI_TIMING_CONFIG_800KBITS();
        case 500000:    // 500 kbps
            return TWAI_TIMING_CONFIG_500KBITS();
        case 250000:    // 250 kbps
            return TWAI_TIMING_CONFIG_250KBITS();
        case 125000:    // 125 kbps
            return TWAI_TIMING_CONFIG_125KBITS();
        case 100000:    // 100 kbps
            return TWAI_TIMING_CONFIG_100KBITS();
        case 50000:     // 50 kbps
            return TWAI_TIMING_CONFIG_50KBITS();
        case 25000:     // 25 kbps
            return TWAI_TIMING_CONFIG_25KBITS();
        default:
            // 对于非标准波特率，使用自定义配置
            debugPrint("[CAN] Warning: Using non-standard baud rate %lu bps, defaulting to 250k config", baud_rate);
            return TWAI_TIMING_CONFIG_250KBITS();
    }
}

twai_filter_config_t CanNetworkService::prepareFilterConfig() const
{
    // 配置为接受所有消息（标准帧）
    // 可以根据需求添加过滤器以提高性能
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    return f_config;
}

void CanNetworkService::processReceivedMessage(const twai_message_t& message)
{
    // 检查消息有效性
    if (message.extd) {
            debugPrint("[CAN] Ignore extended frame ID: 0x%08X", message.identifier);
        return;
    }
    
    if (message.data_length_code != 8) {
        debugPrint("[CAN] Ignore non-standard length message: DLC=%d", message.data_length_code);
        return;
    }
    
    rx_count_++;
    
    // 解析 CAN 消息
    ProtocolResult result;
    bool parse_success = GreenhouseProtocol::parsePacket(
        message.identifier, 
        message.data, 
        result
    );
    
    if (!parse_success) {
        parse_error_count_++;
        debugPrint("[CAN] Protocol parse failed, ID: 0x%03X", message.identifier);
        return;
    }
    
    if (!result.is_valid) {
        parse_error_count_++;
        debugPrint("[CAN] Parse result invalid, ID: 0x%03X", message.identifier);
        return;
    }
    
    // 根据功能码处理消息
    switch (result.func_code) {
        case FUNC_REPORT: // 从节点上报当前数据
            // 更新 SensorState 当前值
            if (sensor_state_.updateState(
                result.node_id, 
                static_cast<uint8_t>(result.param_index), 
                result.scaled_value, 
                false  // 当前值，非目标值
            )) {
                // 标记节点在线
                sensor_state_.markNodeOnline(result.node_id);
                debugPrint("[CAN] Update node %d current value, param %d = %.2f", 
                    result.node_id, result.param_index, result.scaled_value);
            } else {
                debugPrint("[CAN] Update node %d state failed", result.node_id);
            }
            break;
            
        case FUNC_WRITE_SET: // 从节点确认目标设置
            // 更新 SensorState 目标值
            if (sensor_state_.updateState(
                result.node_id, 
                static_cast<uint8_t>(result.param_index), 
                result.scaled_value, 
                true  // 目标值
            )) {
                debugPrint("[CAN] Node %d confirmed target setting, param %d = %.2f", 
                    result.node_id, result.param_index, result.scaled_value);
            } else {
                debugPrint("[CAN] Failed to update node %d target value", result.node_id);
            }
            break;
            
        case FUNC_ALERT: // 告警消息
            debugPrint("[CAN] Node %d Alert, param %d = %.2f", 
                result.node_id, result.param_index, result.scaled_value);
            // 可以添加告警处理逻辑
            break;
            
        case FUNC_TIME_SYNC: // 时间同步
            debugPrint("[CAN] Time sync message from node %d", result.node_id);
            // 可以添加时间同步逻辑
            break;
            
        default:
            debugPrint("[CAN] Unknown function code: %d, node %d", 
                result.func_code, result.node_id);
            break;
    }
}

void CanNetworkService::handleBusError(uint32_t error_flags)
{
    bus_error_count_++;
    
    // 使用 ESP-IDF 的 twai_status_info_t 结构体中的状态信息
    // 注意：error_flags 参数这里未使用，我们直接检查状态结构体
    twai_status_info_t status;
    esp_err_t err = twai_get_status_info(&status);
    if (err != ESP_OK) {
        debugPrint("[CAN] Failed to get bus status: 0x%X", err);
        return;
    }
    
    // 根据状态处理错误
    switch (status.state) {
        case TWAI_STATE_RUNNING:
            debugPrint("[CAN] Bus state: RUNNING");
            break;
        case TWAI_STATE_STOPPED:
            debugPrint("[CAN] Bus state: STOPPED");
            break;
        case TWAI_STATE_BUS_OFF:
            debugPrint("[CAN] Bus state: BUS-OFF");
            debugPrint("[CAN] Attempting bus recovery...");
            attemptBusRecovery();
            break;
        case TWAI_STATE_RECOVERING:
            debugPrint("[CAN] Bus state: RECOVERING");
            break;
        default:
            debugPrint("[CAN] Bus state: Unknown (%d)", status.state);
            break;
    }
    
    // 输出错误计数器信息
    if (status.bus_error_count > 0 || status.tx_error_counter > 0 || status.rx_error_counter > 0) {
        debugPrint("[CAN] Error Counters: BusErr=%d, TXErr=%d, RXErr=%d",
            status.bus_error_count, status.tx_error_counter, status.rx_error_counter);
    }
}

bool CanNetworkService::attemptBusRecovery()
{
    debugPrint("[CAN] Attempting bus recovery...");
    
    // 停止 TWAI 驱动
    if (is_started_) {
        esp_err_t err = twai_stop();
        if (err != ESP_OK) {
            debugPrint("[CAN] Failed to stop TWAI: 0x%X", err);
            return false;
        }
        is_started_ = false;
    }
    
    // 等待一段时间让总线稳定
    OsService::safe_delay_ms(100);
    
    // 重新启动 TWAI 驱动
    esp_err_t err = twai_start();
    if (err != ESP_OK) {
        debugPrint("[CAN] Failed to restart TWAI: 0x%X", err);
        return false;
    }
    
    is_started_ = true;
    debugPrint("[CAN] Bus recovery successful");
    return true;
}

void CanNetworkService::debugPrint(const char* format, ...) const
{
    #ifdef DEBUG_MODE
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    OsService::safe_printf("%s\n", buffer);
    va_end(args);
    #endif
}

// ========================================================================
// 公共方法实现
// ========================================================================

bool CanNetworkService::begin(const CanConfig& config)
{
    // 检查是否已初始化
    if (is_initialized_) {
        debugPrint("[CAN] Warning: CAN service already initialized");
        return true;
    }
    
    // 保存配置
    config_ = config;
    
    debugPrint("[CAN] Initializing CAN network service...");
    debugPrint("[CAN] Config: TX=%d, RX=%d, Baud=%lu bps, Queue=%lu",
        config.tx_pin, config.rx_pin, config.baud_rate, config.queue_size);
    
    // 准备配置结构
    twai_general_config_t g_config = prepareGeneralConfig(config);
    twai_timing_config_t t_config = prepareTimingConfig(config.baud_rate);
    twai_filter_config_t f_config = prepareFilterConfig();
    
    // 安装 TWAI 驱动
    esp_err_t err = twai_driver_install(&g_config, &t_config, &f_config);
    if (err != ESP_OK) {
        debugPrint("[CAN] TWAI driver install failed: 0x%X", err);
        return false;
    }
    
    is_initialized_ = true;
    debugPrint("[CAN] TWAI driver installed");
    
    // 启动 TWAI 驱动
    err = twai_start();
    if (err != ESP_OK) {
        debugPrint("[CAN] TWAI driver start failed: 0x%X", err);
        twai_driver_uninstall();
        is_initialized_ = false;
        return false;
    }
    
    is_started_ = true;
    debugPrint("[CAN] TWAI driver started");
    
    // 重置统计信息
    resetStatistics();
    
    debugPrint("[CAN] CAN service init complete");
    return true;
}

void CanNetworkService::poll()
{
    // 检查是否已初始化
    if (!is_initialized_ || !is_started_) {
        debugPrint("[CAN] Warning: Service not initialized, cannot poll");
        return;
    }
    
    // 检查并处理接收到的消息
    twai_message_t message;
    while (true) {
        esp_err_t err = twai_receive(&message, pdMS_TO_TICKS(0));
        if (err == ESP_ERR_TIMEOUT) {
            // 没有更多消息
            break;
        } else if (err != ESP_OK) {
            // 接收错误
            debugPrint("[CAN] Receive failed: 0x%X", err);
            break;
        } else {
            // 成功接收消息
            processReceivedMessage(message);
        }
    }
    
    // 检查总线错误状态
    twai_status_info_t status;
    esp_err_t err = twai_get_status_info(&status);
    if (err == ESP_OK && status.state != TWAI_STATE_RUNNING) {
        debugPrint("[CAN] Abnormal TWAI state: %d", status.state);
        handleBusError(status.msgs_to_rx);
    } else if (err == ESP_OK && status.state == TWAI_STATE_RUNNING) {
        // 检查是否有错误标志
        if (status.bus_error_count > 0 || status.tx_error_counter > 0 || status.rx_error_counter > 0) {
            // 降低日志频率，防止刷屏（逻辑上需要注意）
            static uint32_t last_log_time = 0;
            if (millis() - last_log_time > 5000) {
                debugPrint("[CAN] Bus errors: bus=%d, tx=%d, rx=%d",
                    status.bus_error_count, status.tx_error_counter, status.rx_error_counter);
                last_log_time = millis();
            }
        }
    }
}

void CanNetworkService::stop()
{
    if (!is_initialized_) {
        return;
    }
    
    debugPrint("[CAN] Stopping CAN service...");
    
    // 停止 TWAI 驱动
    if (is_started_) {
        esp_err_t err = twai_stop();
        if (err != ESP_OK) {
            debugPrint("[CAN] Failed to stop TWAI: 0x%X", err);
        } else {
            debugPrint("[CAN] TWAI driver stopped");
        }
        is_started_ = false;
    }
    
    // 卸载 TWAI 驱动
    esp_err_t err = twai_driver_uninstall();
    if (err != ESP_OK) {
        debugPrint("[CAN] Failed to uninstall TWAI: 0x%X", err);
    } else {
        debugPrint("[CAN] TWAI driver uninstalled");
    }
    
    is_initialized_ = false;
    debugPrint("[CAN] CAN service stopped");
}

bool CanNetworkService::sendControl(uint8_t node_id, ParameterIndex param_index, float value)
{
    // 检查是否已初始化
    if (!is_initialized_ || !is_started_) {
        debugPrint("[CAN] Error: Service not initialized, cannot send");
        return false;
    }
    
    // 验证参数
    if (node_id > MAX_NODE_ID) {
        debugPrint("[CAN] Error: Invalid Node ID %d (Max %d)", node_id, MAX_NODE_ID);
        return false;
    }
    
    if (!GreenhouseProtocol::isValidIndex(param_index)) {
        debugPrint("[CAN] Error: Invalid param index %d", param_index);
        return false;
    }
    
    // 构建 CAN 消息
    uint8_t can_data[8];
    GreenhouseProtocol::buildPacket(can_data, param_index, value);
    
    // 构建 CAN ID (FUNC_WRITE_SET 功能码)
    uint16_t can_id = GreenhouseProtocol::buildID(FUNC_WRITE_SET, node_id);
    
    // 准备 TWAI 消息
    twai_message_t message;
    message.identifier = can_id;
    message.extd = 0;  // 标准帧
    message.rtr = 0;   // 数据帧
    message.data_length_code = 8;
    memcpy(message.data, can_data, 8);
    
    // 发送消息
    esp_err_t err = twai_transmit(&message, pdMS_TO_TICKS(100));
    if (err != ESP_OK) {
        debugPrint("[CAN] Failed to send control command: 0x%X", err);
        return false;
    }
    
    tx_count_++;
    debugPrint("[CAN] Control command sent: Node %d, Param %d, Val %.2f",
        node_id, param_index, value);
    return true;
}

bool CanNetworkService::sendRaw(uint16_t id, const uint8_t* data, size_t data_length)
{
    // 检查是否已初始化
    if (!is_initialized_ || !is_started_) {
        debugPrint("[CAN] Error: Service not initialized, cannot send");
        return false;
    }
    
    // 验证参数
    if (data == nullptr || data_length != 8) {
        debugPrint("[CAN] Error: Invalid data, length must be 8");
        return false;
    }
    
    if (id > 0x7FF) {  // 11-bit CAN ID
        debugPrint("[CAN] Error: Invalid CAN ID 0x%X", id);
        return false;
    }
    
    // 准备 TWAI 消息
    twai_message_t message;
    message.identifier = id;
    message.extd = 0;  // 标准帧
    message.rtr = 0;   // 数据帧
    message.data_length_code = static_cast<uint8_t>(data_length);
    memcpy(message.data, data, data_length);
    
    // 发送消息
    esp_err_t err = twai_transmit(&message, pdMS_TO_TICKS(100));
    if (err != ESP_OK) {
        debugPrint("[CAN] Failed to send raw message: 0x%X", err);
        return false;
    }
    
    tx_count_++;
    debugPrint("[CAN] Raw message sent, ID: 0x%03X", id);
    return true;
}

twai_status_info_t CanNetworkService::getStatus() const
{
    twai_status_info_t status = {};
    if (is_initialized_) {
        esp_err_t err = twai_get_status_info(&status);
        if (err != ESP_OK) {
            debugPrint("[CAN] Failed to get status: 0x%X", err);
        }
    }
    return status;
}

void CanNetworkService::resetStatistics()
{
    rx_count_ = 0;
    tx_count_ = 0;
    parse_error_count_ = 0;
    bus_error_count_ = 0;
    debugPrint("[CAN] Statistics reset");
}

void CanNetworkService::printDiagnostics() const
{
    debugPrint("=== CAN Network Service Diagnostics ===");
    debugPrint("Status: %s", is_initialized_ ? (is_started_ ? "RUNNING" : "STOPPED") : "UNINITIALIZED");
    
    if (is_initialized_) {
        debugPrint("Config: TX=GPIO%d, RX=GPIO%d, Baud=%lu bps",
            config_.tx_pin, config_.rx_pin, config_.baud_rate);
        
        twai_status_info_t status = getStatus();
        debugPrint("TWAI State: %s", 
            status.state == TWAI_STATE_RUNNING ? "RUNNING" :
            status.state == TWAI_STATE_STOPPED ? "STOPPED" :
            status.state == TWAI_STATE_BUS_OFF ? "BUS-OFF" :
            status.state == TWAI_STATE_RECOVERING ? "RECOVERING" : "UNKNOWN");
        
        debugPrint("Error Counters: TX=%d, RX=%d, BusErr=%d",
            status.tx_error_counter, status.rx_error_counter, status.bus_error_count);
        
        debugPrint("Msg Stats: RX=%lu, TX=%lu",
            status.msgs_to_rx, status.msgs_to_tx);
    }
    
    debugPrint("App Stats: RX=%lu, TX=%lu, ParseErr=%lu, BusErr=%lu",
        rx_count_, tx_count_, parse_error_count_, bus_error_count_);
    debugPrint("=======================================");
}

} // namespace Service