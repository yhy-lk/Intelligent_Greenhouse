/**
 * @file can_network_service.h
 * @brief CAN 网络服务层 - 硬件抽象与协议桥接 (纯 C 重构版)
 * * 此模块作为 ESP32 TWAI/CAN 硬件驱动与业务逻辑层之间的桥梁。
 * 负责硬件初始化、消息收发、调用协议层解析报文，并将有效数据同步至全局数字孪生状态。
 * * 架构数据通路：
 * [底层 TWAI 驱动] <--> [本服务层 poll/send] <--> [can_protocol 解析] <--> [sensor_state 全局状态] <--> [LVGL UI]
 * * 核心特性:
 * - 纯 C 实现，告别 C++ 依赖注入，彻底打通 LVGL 到 CAN 总线的调用壁垒。
 * - 隐藏硬件复杂性：将 TWAI 驱动的繁杂配置封装为易用的 API。
 * - 健壮的错误处理：内置总线 off 恢复机制和丢包/错误统计。
 * * @note 【中断与并发安全警告】
 * 1. 发送 API (can_service_send_xxx) 内部封装了队列等待，允许 LVGL 任务等多个任务并发调用。
 * 2. 轮询 API (can_service_poll) 应由一个独立的 FreeRTOS 任务独占并循环调用。
 * 3. 严禁在任何硬件中断 (ISR) 中调用本文件的任何函数。
 */

#ifndef CAN_NETWORK_SERVICE_H
#define CAN_NETWORK_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

// ESP-IDF TWAI 驱动原生头文件
#include "driver/twai.h"
#include "hal/gpio_types.h"

// 引入依赖的协议层定义 (获取 CanParamIndex 等类型)
#include "can_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 数据结构定义 (Rust 风格类型名)
// ============================================================================

/**
 * @brief CAN 硬件配置参数结构体
 * 包含 TWAI 驱动初始化所需的所有底层参数，避免硬编码。
 */
typedef struct {
    gpio_num_t  tx_pin;       /**< TX 引脚号 (例如 GPIO_NUM_47) */
    gpio_num_t  rx_pin;       /**< RX 引脚号 (例如 GPIO_NUM_48) */
    uint32_t    baud_rate;    /**< 波特率 (bps)，例如 250000, 500000 */
    uint32_t    queue_size;   /**< 接收与发送队列的最大消息缓存数量，默认建议 20 */
    twai_mode_t mode;         /**< TWAI 工作模式，通常为 TWAI_MODE_NORMAL */
} CanConfig;

/**
 * @brief 运行状态与错误统计指标结构体
 * 用于 UI 诊断界面或系统监控任务。
 */
typedef struct {
    uint32_t rx_count;          /**< 成功接收并处理的 CAN 消息总数 */
    uint32_t tx_count;          /**< 成功发送的 CAN 消息总数 */
    uint32_t parse_error_count; /**< 协议解析失败(非法帧/校验错)的计数 */
    uint32_t bus_error_count;   /**< 物理总线错误事件计数 */
} CanMetrics;

// ============================================================================
// 默认配置宏辅助
// ============================================================================

/** * @brief 快速生成默认 CAN 配置的宏 
 * @param tx 指定 TX 引脚
 * @param rx 指定 RX 引脚
 * @param baud 指定波特率 (bps)
 */
#define CAN_CONFIG_DEFAULT(tx, rx, baud) \
    { \
        .tx_pin = (tx), \
        .rx_pin = (rx), \
        .baud_rate = (baud), \
        .queue_size = 20, \
        .mode = TWAI_MODE_NORMAL \
    }

// ============================================================================
// 生命周期管理 API
// ============================================================================

/**
 * @brief 初始化并启动 CAN 网络服务
 * @param config 指向 CAN 硬件配置结构体的指针
 * @return true 驱动安装且启动成功
 * @return false 驱动初始化失败
 * @note 必须在调用 poll 或任何 send 函数之前调用此函数。
 */
bool can_service_init(const CanConfig* config);

/**
 * @brief 停止并卸载 CAN 服务
 * @note 释放底层 TWAI 驱动的内存资源。
 */
void can_service_stop(void);

/**
 * @brief 检查 CAN 服务是否处于就绪状态
 * @return true 服务已启动并正常运行
 * @return false 服务未初始化或已停止
 */
bool can_service_is_initialized(void);

// ============================================================================
// 核心调度 API (通常驻留在独立的 FreeRTOS Task 中)
// ============================================================================

/**
 * @brief 轮询处理 CAN 接收队列与总线状态 (核心引擎)
 * * 此函数会非阻塞或短暂阻塞地检查 TWAI 接收队列。
 * 一旦收到有效报文，会自动调用 `can_protocol` 解包，并立刻通过 
 * `sensor_state_update_xxx` 接口将物理值同步到数字孪生全局单例中。
 * 同时它还会监控总线 Bus-Off 状态并自动尝试恢复。
 * * @note 强烈建议将此函数放在一个独立的 FreeRTOS 任务的主循环中，
 * 例如使用 vTaskDelay(pdMS_TO_TICKS(10)) 进行周期性调用。
 */
void can_service_poll(void);

// ============================================================================
// 数据发送 API (供 LVGL 或控制逻辑层调用)
// ============================================================================

/**
 * @brief 向指定节点下发控制/设定指令
 * * 此函数会自动查询参数索引对应的缩放因子，将物理浮点数打包为 CAN 帧，
 * 使用 FUNC_WRITE_SET 功能码，并放入硬件发送队列。
 * * @param node_id 目标节点 ID (0-127)
 * @param param_index 控制参数索引 (例如 PARAM_IDX_WATER_PUMP)
 * @param physical_value 期望设定的物理值 (例如 1.0f 表示开，0.0f 表示关)
 * @return true 发送队列成功接收该请求
 * @return false 服务未就绪、参数非法或发送队列已满
 * * @note LVGL 按钮的回调函数可以直接调用此接口，它是线程安全的。
 */
bool can_service_send_control(uint8_t node_id, CanParamIndex param_index, float physical_value);

/**
 * @brief 发送底层原始 CAN 报文 (透传接口)
 * * 适用于无需经过协议字典解析的特殊报文，如系统时间同步包等。
 * * @param can_id 11-bit CAN ID
 * @param data 指向 8 字节数据的指针
 * @param data_length 数据长度 (必须为 8，严格遵守定长协议)
 * @return true 发送成功
 * @return false 发送失败
 */
bool can_service_send_raw(uint16_t can_id, const uint8_t* data, size_t data_length);

// ============================================================================
// 诊断与状态监控 API
// ============================================================================

/**
 * @brief 获取当前的通信诊断指标 (统计数量)
 * @param out_metrics 指向用于接收统计数据的结构体指针
 */
void can_service_get_metrics(CanMetrics* out_metrics);

/**
 * @brief 获取底层 TWAI 驱动的原生状态
 * @param out_status 指向 TWAI 状态结构体的指针
 * @return true 获取成功
 * @return false 获取失败 (驱动未初始化)
 */
bool can_service_get_twai_status(twai_status_info_t* out_status);

/**
 * @brief 清零所有统计计数器
 */
void can_service_reset_metrics(void);

/**
 * @brief 在串口控制台打印格式化的 CAN 服务诊断报告
 */
void can_service_print_diagnostics(void);

#ifdef __cplusplus
}
#endif

#endif // CAN_NETWORK_SERVICE_H