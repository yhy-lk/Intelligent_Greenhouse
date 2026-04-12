/**
 * @file can_protocol.c
 * @brief 智能温室 CAN 总线协议层实现
 * * 此文件包含 CAN 报文打包、解包、校验以及物理值与原始数据互相转换的具体实现。
 * 针对 ESP32-S3 等小端序 (Little-Endian) 处理器进行了内存映射优化。
 */

#include "can_protocol.h"
#include <string.h>

// ============================================================================
// 私有静态辅助函数 (仅在本文件内部可见)
// ============================================================================

/**
 * @brief 检查参数索引是否在定义的字典中
 */
static bool is_valid_param_index(CanParamIndex index) {
    switch (index) {
        // 系统参数
        case PARAM_IDX_SYSTEM_TIMESTAMP:
        case PARAM_IDX_HEARTBEAT:
        case PARAM_IDX_ERROR_CODE:
        case PARAM_IDX_CONTROL_MODE:
        
        // 开关参数
        case PARAM_IDX_LIGHT_MAIN_POWER:
        case PARAM_IDX_WATER_PUMP:
        case PARAM_IDX_HUMIDIFIER:
        case PARAM_IDX_VENTILATION_FAN:
        case PARAM_IDX_SUNSHADE_MOTOR:
        case PARAM_IDX_HEATER:
        case PARAM_IDX_WINDOW_ACTUATOR:
        
        // 模拟量参数
        case PARAM_IDX_TEMPERATURE:
        case PARAM_IDX_HUMIDITY_AIR:
        case PARAM_IDX_HUMIDITY_SOIL:
        case PARAM_IDX_LIGHT_INTENSITY:
        case PARAM_IDX_CO2_LEVEL:
        case PARAM_IDX_FAN_SPEED:
        case PARAM_IDX_SOIL_PH:
        case PARAM_IDX_SOIL_EC:
        case PARAM_IDX_WATER_LEVEL:
        
        // 高级参数
        case PARAM_IDX_LIGHT_COLOR_RGB:
        case PARAM_IDX_LIGHT_PWM_DUTY:
            return true;
            
        default:
            return false;
    }
}

/**
 * @brief 检查 CAN 帧的保留字节是否全部为 0
 */
static bool are_reserved_bytes_zero(const uint8_t* reserved) {
    return (reserved[0] == 0) && (reserved[1] == 0) && (reserved[2] == 0);
}

/**
 * @brief 获取特定参数的缩放因子
 */
static float get_scaling_factor(CanParamIndex index) {
    switch (index) {
        case PARAM_IDX_TEMPERATURE:
        case PARAM_IDX_HUMIDITY_AIR:
        case PARAM_IDX_HUMIDITY_SOIL:
        case PARAM_IDX_SOIL_PH:
            return 100.0f; // 放大 100 倍传输，保留两位小数
            
        default:
            return 1.0f;   // 1:1 传输
    }
}

/**
 * @brief 将物理浮点值转换为底层传输所需的整形值 (带四舍五入与负数支持)
 */
static uint32_t float_to_raw_u32(CanParamIndex index, float physical_value) {
    float factor = get_scaling_factor(index);
    float scaled = physical_value * factor;
    
    // 使用 int32_t 作为中转，完美支持零下温度等负数情况 (利用补码特性)
    int32_t int_val;
    if (scaled >= 0.0f) {
        int_val = (int32_t)(scaled + 0.5f); // 正数四舍五入
    } else {
        int_val = (int32_t)(scaled - 0.5f); // 负数四舍五入
    }
    
    return (uint32_t)int_val;
}

/**
 * @brief 将底层传输的整形值还原为物理浮点值
 */
static float raw_u32_to_float(CanParamIndex index, uint32_t raw_value) {
    float factor = get_scaling_factor(index);
    
    if (factor == 0.0f) {
        return 0.0f; // 防呆设计，避免除零异常
    }
    
    // 将 uint32_t 强制转回 int32_t，正确还原负数 (如温度 -15.5℃)
    int32_t int_val = (int32_t)raw_value;
    
    return (float)int_val / factor;
}

/**
 * @brief 校验原始 CAN 帧结构的合法性
 */
static bool validate_can_frame(const CanDataFrame* frame) {
    if (frame == NULL) {
        return false;
    }
    
    // 强制要求保留位为 0，防止脏数据或者版本不兼容
    if (!are_reserved_bytes_zero(frame->reserved)) {
        return false;
    }
    
    // 检查参数索引是否越界
    if (!is_valid_param_index((CanParamIndex)frame->index)) {
        return false;
    }
    
    return true;
}

// ============================================================================
// CAN ID 操作 API 实现
// ============================================================================

uint16_t can_proto_build_id(CanFuncCode func, uint8_t node_id) {
    // 掩码过滤，防止越界数据污染其他位域
    uint8_t safe_func = ((uint8_t)func) & CAN_ID_MASK_FUNC;
    uint8_t safe_node = node_id & CAN_ID_MASK_NODE;
    
    return (uint16_t)((safe_func << CAN_ID_FUNC_SHIFT) | safe_node);
}

CanFuncCode can_proto_get_func(uint16_t id) {
    return (CanFuncCode)((id >> CAN_ID_FUNC_SHIFT) & CAN_ID_MASK_FUNC);
}

uint8_t can_proto_get_node(uint16_t id) {
    return (uint8_t)(id & CAN_ID_MASK_NODE);
}

// ============================================================================
// 报文打包与解析 API 实现
// ============================================================================

bool can_proto_parse_packet(uint32_t can_id, const uint8_t* data, CanParsedPacket* out_result) {
    if (out_result == NULL) {
        return false;
    }
    
    // 初始化输出结果内存
    memset(out_result, 0, sizeof(CanParsedPacket));
    out_result->is_valid = false;
    out_result->param_index = PARAM_IDX_INVALID;
    
    if (data == NULL) {
        return false;
    }
    
    // 提取并校验 11 位标准帧 ID
    uint16_t std_id = can_id & 0x7FF;
    out_result->func_code = can_proto_get_func(std_id);
    out_result->node_id = can_proto_get_node(std_id);
    
    if (out_result->func_code >= CAN_FUNC_COUNT) {
        return false; // 非法的功能码
    }
    
    // 内存对齐映射：直接将 byte 数组视作结构体 (ESP32 是小端序，这里性能极高，0 拷贝)
    const CanDataFrame* frame = (const CanDataFrame*)data;
    
    // 帧校验
    if (!validate_can_frame(frame)) {
        return false;
    }
    
    // 数据提取
    out_result->param_index = (CanParamIndex)frame->index;
    out_result->raw_value = frame->value;
    out_result->scaled_value = raw_u32_to_float(out_result->param_index, out_result->raw_value);
    out_result->is_scaled = (get_scaling_factor(out_result->param_index) != 1.0f);
    
    out_result->is_valid = true;
    return true;
}

void can_proto_build_packet(uint8_t* out_buffer, CanParamIndex index, float physical_value) {
    if (out_buffer == NULL) {
        return;
    }
    
    // 转换为底层整形
    uint32_t raw_val = float_to_raw_u32(index, physical_value);
    
    // 内存映射直接赋值
    CanDataFrame* frame = (CanDataFrame*)out_buffer;
    frame->index = (uint8_t)index;
    frame->reserved[0] = 0;
    frame->reserved[1] = 0;
    frame->reserved[2] = 0;
    frame->value = raw_val; // ESP32 小端架构会自动处理内存字节序
}

// ============================================================================
// 调试与诊断辅助 API 实现
// ============================================================================

const char* can_proto_get_param_name(CanParamIndex index) {
    switch (index) {
        case PARAM_IDX_SYSTEM_TIMESTAMP:  return "System Timestamp";
        case PARAM_IDX_HEARTBEAT:         return "Heartbeat";
        case PARAM_IDX_ERROR_CODE:        return "Error Code";
        case PARAM_IDX_CONTROL_MODE:      return "Control Mode";
        
        case PARAM_IDX_LIGHT_MAIN_POWER:  return "Light Main Power";
        case PARAM_IDX_WATER_PUMP:        return "Water Pump";
        case PARAM_IDX_HUMIDIFIER:        return "Humidifier";
        case PARAM_IDX_VENTILATION_FAN:   return "Ventilation Fan";
        case PARAM_IDX_SUNSHADE_MOTOR:    return "Sunshade Motor";
        case PARAM_IDX_HEATER:            return "Heater";
        case PARAM_IDX_WINDOW_ACTUATOR:   return "Window Actuator";
        
        case PARAM_IDX_TEMPERATURE:       return "Temperature";
        case PARAM_IDX_HUMIDITY_AIR:      return "Air Humidity";
        case PARAM_IDX_HUMIDITY_SOIL:     return "Soil Humidity";
        case PARAM_IDX_LIGHT_INTENSITY:   return "Light Intensity";
        case PARAM_IDX_CO2_LEVEL:         return "CO2 Level";
        case PARAM_IDX_FAN_SPEED:         return "Fan Speed";
        case PARAM_IDX_SOIL_PH:           return "Soil pH";
        case PARAM_IDX_SOIL_EC:           return "Soil EC";
        case PARAM_IDX_WATER_LEVEL:       return "Water Level";
        
        case PARAM_IDX_LIGHT_COLOR_RGB:   return "Light Color RGB";
        case PARAM_IDX_LIGHT_PWM_DUTY:    return "Light PWM Duty";
        
        default:                          return "Unknown Parameter";
    }
}

const char* can_proto_get_func_name(CanFuncCode func) {
    switch (func) {
        case CAN_FUNC_ALERT:     return "Alert";
        case CAN_FUNC_TIME_SYNC: return "Time Sync";
        case CAN_FUNC_WRITE_SET: return "Write/Set";
        case CAN_FUNC_REPORT:    return "Report";
        default:                 return "Unknown Function";
    }
}