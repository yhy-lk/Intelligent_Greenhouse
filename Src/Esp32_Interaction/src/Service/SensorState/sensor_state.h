/**
 * @file sensor_state.h
 * @brief 智能温室数字孪生 - 中间件服务层 (纯 C 重构版)
 * * 本模块为智能温室系统实现了一个“数字孪生”状态服务层。
 * 它维护了所有节点当前的传感器读数（真实状态）和目标设定值（期望状态），
 * 在底层的 CAN 协议层与上层的 JSON/LVGL UI 逻辑层之间提供了一个干净的抽象接口。
 * * 架构定位:
 * - 上层: LVGL UI 层 / JSON 业务逻辑层 (标准化的数据访问，无锁等待)
 * - 下层: CAN 协议层 (原始 CAN 数据的解析与写入)
 * * 核心特性 (v2.0):
 * - 纯 C 语言实现：完美兼容 LVGL，消除了 C++ 命名空间和类的耦合。
 * - 线程安全：内部封装了 FreeRTOS 互斥锁 (Mutex)，保证多任务并发读写的原子性。
 * - UI 防撕裂：提供整体“快照(Snapshot)”读取机制，防止 LVGL 渲染时数据不一致。
 * - 内存优化：使用固定大小的静态数组（空间换时间），无任何动态内存分配 (malloc)。
 * * @warning 【严重警告 - 中断安全】
 * 本模块内部的所有读写 API 均使用了 FreeRTOS 的互斥锁 (xSemaphoreTake/Give)。
 * 绝对禁止在任何中断服务例程 (ISR) 中调用本文件中的任何函数！
 * 如果在 ISR 中调用，将导致 FreeRTOS 调度器崩溃或触发看门狗复位。
 */

#ifndef SENSOR_STATE_H
#define SENSOR_STATE_H

#include <stdint.h>
#include <stdbool.h>

// 复用 CAN 协议的参数索引宏定义
#include "can_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 配置常量定义
// ============================================================================

/** 系统中允许的最大节点数量 (NodeID: 0-15) */
#define MAX_NODES 16

/**
 * @brief 用于数组大小分配的最大参数索引值
 * * 现有的参数索引范围是从 0x00 到 0x51（0x51 = INDEX_LIGHT_PWM_DUTY）。
 * 使用 0x60 (96) 可以为未来的功能扩展提供安全的内存填充边界，
 * 同时在 ESP32-S3 上保持合理的内存使用率。
 */
#define MAX_PARAM_INDEX 0x60

/** 表示数据无效、未初始化或传感器掉线的特殊值 */
#define INVALID_VALUE (-9999.0f)

/** 默认的节点掉线超时时间（毫秒）。默认 5 分钟 (5 * 60 * 1000) */
#define DEFAULT_NODE_TIMEOUT_MS 300000

// ============================================================================
// 数据结构定义
// ============================================================================

/**
 * @brief 单个节点的数字孪生状态数据
 * * 该结构体维护了单个温室节点（设备）的所有参数的当前物理读数和期望目标设定。
 * * 数字孪生概念:
 * - current_values: 物理设备当前真实反馈的传感器读数
 * - target_values: 用户通过 UI 或 AI 控制器下发的期望设定值
 */
typedef struct {
    bool is_online;                 /**< 节点当前是否在线 */
    uint32_t last_seen_timestamp;   /**< 最后一次成功通信的系统时间戳 (毫秒) */
    
    /**
     * @brief 当前传感器读数 (物理设备的实际值)
     * * 数组通过参数索引 (0x00 到 MAX_PARAM_INDEX-1) 进行访问。
     * 为了方便业务逻辑层处理，所有值均统一存储为 float 浮点数。
     * 开关量状态存储为 0.0f (OFF) 或 1.0f (ON)。
     * * @note 未初始化或当前不可用的参数其值为 INVALID_VALUE。
     */
    float current_values[MAX_PARAM_INDEX];
    
    /**
     * @brief 目标设定值 (用户/AI 期望达到的值)
     * * 数组通过参数索引 (0x00 到 MAX_PARAM_INDEX-1) 进行访问。
     * 代表系统应当努力调节达到的目标状态。
     * * @note 没有目标设定的参数其值为 INVALID_VALUE。
     */
    float target_values[MAX_PARAM_INDEX];
} NodeData;

// ============================================================================
// 生命周期管理 API
// ============================================================================

/**
 * @brief 初始化传感器状态模块
 * * 创建内部的 FreeRTOS 互斥锁，并将所有节点的数据格式化为 INVALID_VALUE。
 * @note 必须在 FreeRTOS 调度器启动前或在系统初始化早期调用此函数！
 */
void sensor_state_init(void);

// ============================================================================
// 数据写入 API (供 CAN 协议层/数据接收 Task 调用)
// ============================================================================

/**
 * @brief 更新节点的当前传感器读数
 * * 当 CAN 接收任务解析到有效的传感器上报报文时调用此函数。
 * 内部会自动获取互斥锁，并更新节点的在线状态和时间戳。
 * * @param node_id 节点 ID (0-15)
 * @param param_index 参数索引 (例如 0x30 表示温度)
 * @param value 已转换为物理单位的浮点数值 (如 °C, %)
 * @return true 更新成功
 * @return false 更新失败 (非法的 node_id 或 param_index)
 */
bool sensor_state_update_current(uint8_t node_id, uint8_t param_index, float value);

/**
 * @brief 更新节点的目标设定值
 * * 当 CAN 接收任务收到节点对设定指令的 ACK 确认时调用。
 * * @param node_id 节点 ID (0-15)
 * @param param_index 参数索引
 * @param value 目标设定的浮点数值
 * @return true 更新成功
 * @return false 更新失败
 */
bool sensor_state_update_target(uint8_t node_id, uint8_t param_index, float value);

/**
 * @brief 仅将节点标记为在线并刷新时间戳 (心跳包处理)
 * * @param node_id 节点 ID (0-15)
 * @return true 成功
 * @return false 非法的节点 ID
 */
bool sensor_state_mark_online(uint8_t node_id);

// ============================================================================
// 数据读取 API (供 LVGL 渲染 Task 或业务逻辑层调用)
// ============================================================================

/**
 * @brief [核心优化] 获取整个节点数据的完整快照 (专为 LVGL 设计)
 * * 此函数会一次性获取互斥锁，将指定节点的所有状态 (NodeData) 
 * 内存拷贝 (memcpy) 到传入的结构体指针中，然后立即释放锁。
 * * @note LVGL 渲染 UI 时，强烈建议使用此函数。它可以防止在渲染多个参数（如温湿度）
 * 的过程中，底层 CAN 任务更新了数据导致的“UI 撕裂 (Data Tearing)”问题。
 * * @param node_id 要获取的节点 ID (0-15)
 * @param out_data 指向用户分配的 NodeData 结构体的指针，用于接收拷贝数据
 * @return true 拷贝成功
 * @return false 失败 (非法的节点 ID，或 out_data 为空指针)
 */
bool sensor_state_get_node_snapshot(uint8_t node_id, NodeData* out_data);

/**
 * @brief 获取单个参数的当前读数 (单点读取)
 * * 内部包含互斥锁保护。适用于只关心某一个特定参数的简单逻辑判定。
 * * @param node_id 节点 ID (0-15)
 * @param param_index 参数索引
 * @return float 传感器读数；如果节点非法或参数不可用，返回 INVALID_VALUE
 */
float sensor_state_get_current(uint8_t node_id, uint8_t param_index);

/**
 * @brief 获取单个参数的目标设定值 (单点读取)
 * * @param node_id 节点 ID (0-15)
 * @param param_index 参数索引
 * @return float 目标设定值；如果节点非法或未设置，返回 INVALID_VALUE
 */
float sensor_state_get_target(uint8_t node_id, uint8_t param_index);

/**
 * @brief 检查指定节点当前是否在线
 * * 基于内部记录的时间戳与 FreeRTOS 的当前系统滴答(Tick)进行比对，
 * 如果超过 DEFAULT_NODE_TIMEOUT_MS 则判定为离线。
 * * @param node_id 节点 ID (0-15)
 * @return true 节点在线 (未超时)
 * @return false 节点离线或非法 ID
 */
bool sensor_state_is_online(uint8_t node_id);

#ifdef __cplusplus
}
#endif

#endif // SENSOR_STATE_H