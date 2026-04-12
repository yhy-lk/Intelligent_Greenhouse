/**
 * @file sensor_state.c
 * @brief 智能温室数字孪生 - 中间件服务层 (纯 C 实现)
 * * @note 【中断安全警告】
 * 本文件中的所有功能型 API（除 init 外）均使用了 xSemaphoreTake/Give。
 * 严禁在硬件定时器中断、GPIO 外部中断、CAN 接收中断等 ISR 上下文中调用！
 */

#include "sensor_state.h"
#include <string.h>

// FreeRTOS 头文件 (ESP-IDF 标准包含方式)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

// ============================================================================
// 私有静态数据 (数据隐藏，外部绝对无法直接访问)
// ============================================================================

/** 节点状态的全局存储数组 */
static NodeData s_nodes[MAX_NODES];

/** 用于保护 s_nodes 的互斥锁句柄 */
static SemaphoreHandle_t s_mutex = NULL;

// ============================================================================
// 私有辅助函数 (无锁，仅供内部调用)
// ============================================================================

/**
 * @brief 验证节点 ID 是否合法
 */
static inline bool is_valid_node(uint8_t node_id) {
    return node_id < MAX_NODES;
}

/**
 * @brief 验证参数索引是否合法
 */
static inline bool is_valid_param(uint8_t param_index) {
    return param_index < MAX_PARAM_INDEX;
}

/**
 * @brief 获取当前的系统运行时间(毫秒)
 * @note 使用 FreeRTOS 的 Tick 计数转换，不受 RTC 影响，适合做超时判断
 */
static inline uint32_t get_current_time_ms(void) {
    // pdTICKS_TO_MS 宏在 1000Hz tick_rate 下等同于直接取值，但在跨平台时更安全
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

// ============================================================================
// 公开 API 实现
// ============================================================================

void sensor_state_init(void) {
    // 1. 创建互斥锁 (如果尚未创建)
    if (s_mutex == NULL) {
        s_mutex = xSemaphoreCreateMutex();
    }
    
    // 如果系统资源耗尽导致锁创建失败，属于致命错误
    if (s_mutex == NULL) {
        return; 
    }
    
    // 2. 初始化所有节点数据
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        s_nodes[i].is_online = false;
        s_nodes[i].last_seen_timestamp = 0;
        
        for (uint16_t j = 0; j < MAX_PARAM_INDEX; j++) {
            s_nodes[i].current_values[j] = INVALID_VALUE;
            s_nodes[i].target_values[j] = INVALID_VALUE;
        }
    }
    xSemaphoreGive(s_mutex);
}

bool sensor_state_update_current(uint8_t node_id, uint8_t param_index, float value) {
    if (s_mutex == NULL || !is_valid_node(node_id) || !is_valid_param(param_index)) {
        return false;
    }
    
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    
    s_nodes[node_id].current_values[param_index] = value;
    s_nodes[node_id].is_online = true;
    s_nodes[node_id].last_seen_timestamp = get_current_time_ms();
    
    xSemaphoreGive(s_mutex);
    return true;
}

bool sensor_state_update_target(uint8_t node_id, uint8_t param_index, float value) {
    if (s_mutex == NULL || !is_valid_node(node_id) || !is_valid_param(param_index)) {
        return false;
    }
    
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    
    s_nodes[node_id].target_values[param_index] = value;
    s_nodes[node_id].is_online = true;
    s_nodes[node_id].last_seen_timestamp = get_current_time_ms();
    
    xSemaphoreGive(s_mutex);
    return true;
}

bool sensor_state_mark_online(uint8_t node_id) {
    if (s_mutex == NULL || !is_valid_node(node_id)) {
        return false;
    }
    
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    
    s_nodes[node_id].is_online = true;
    s_nodes[node_id].last_seen_timestamp = get_current_time_ms();
    
    xSemaphoreGive(s_mutex);
    return true;
}

bool sensor_state_get_node_snapshot(uint8_t node_id, NodeData* out_data) {
    if (s_mutex == NULL || !is_valid_node(node_id) || out_data == NULL) {
        return false;
    }
    
    // 一次性加锁，拷贝整块内存 (大约 700+ 字节)，然后立即解锁。
    // 在 ESP32-S3 上，memcpy 几百字节的速度远快于频繁上下文切换的开销。
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    memcpy(out_data, &s_nodes[node_id], sizeof(NodeData));
    xSemaphoreGive(s_mutex);
    
    return true;
}

float sensor_state_get_current(uint8_t node_id, uint8_t param_index) {
    if (s_mutex == NULL || !is_valid_node(node_id) || !is_valid_param(param_index)) {
        return INVALID_VALUE;
    }
    
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    float value = s_nodes[node_id].current_values[param_index];
    xSemaphoreGive(s_mutex);
    
    return value;
}

float sensor_state_get_target(uint8_t node_id, uint8_t param_index) {
    if (s_mutex == NULL || !is_valid_node(node_id) || !is_valid_param(param_index)) {
        return INVALID_VALUE;
    }
    
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    float value = s_nodes[node_id].target_values[param_index];
    xSemaphoreGive(s_mutex);
    
    return value;
}

bool sensor_state_is_online(uint8_t node_id) {
    if (s_mutex == NULL || !is_valid_node(node_id)) {
        return false;
    }
    
    uint32_t current_time = get_current_time_ms();
    bool is_online = false;
    
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    
    // 如果节点曾经上线过，我们需要判断是否超时
    if (s_nodes[node_id].is_online) {
        // 利用无符号整数减法的特性，天然处理了 uint32_t 溢出翻转的问题 (约 49.7 天翻转一次)
        uint32_t time_since_last_seen = current_time - s_nodes[node_id].last_seen_timestamp;
        
        if (time_since_last_seen > DEFAULT_NODE_TIMEOUT_MS) {
            // 节点超时，状态转为离线。此处直接在锁内更新状态，保证一致性。
            s_nodes[node_id].is_online = false;
        } else {
            is_online = true; // 仍在超时时间内，判定为在线
        }
    }
    
    xSemaphoreGive(s_mutex);
    
    return is_online;
}