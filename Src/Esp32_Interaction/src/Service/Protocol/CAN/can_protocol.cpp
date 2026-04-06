/**
 * @file greenhouse_protocol.cpp
 * @brief Implementation of Greenhouse CAN Bus Protocol
 * 
 * This module implements the CAN bus communication protocol for greenhouse automation systems.
 * It provides functions for packing/unpacking CAN messages, converting between physical
 * values and raw CAN data, and handling the protocol's specific data formats.
 */

#include "internal_can_protocol.h"
#include <string.h>
#include <math.h>

// ============================================================================
// Static Helper Functions
// ============================================================================

/**
 * @brief Check if a parameter index is valid
 * 
 * @param index Parameter index to check
 * @return true Index is valid
 * @return false Index is invalid
 */
static bool is_valid_index(ParameterIndex index) {
    switch (index) {
        // System parameters
        case INDEX_SYSTEM_TIMESTAMP:
        case INDEX_HEARTBEAT:
        case INDEX_ERROR_CODE:
        case INDEX_CONTROL_MODE:
        
        // Switch parameters
        case INDEX_LIGHT_MAIN_POWER:
        case INDEX_WATER_PUMP:
        case INDEX_HUMIDIFIER:
        case INDEX_VENTILATION_FAN:
        case INDEX_SUNSHADE_MOTOR:
        case INDEX_HEATER:
        case INDEX_WINDOW_ACTUATOR:
        
        // Analog values
        case INDEX_TEMPERATURE:
        case INDEX_HUMIDITY_AIR:
        case INDEX_HUMIDITY_SOIL:
        case INDEX_LIGHT_INTENSITY:
        case INDEX_CO2_LEVEL:
        case INDEX_FAN_SPEED:
        case INDEX_SOIL_PH:
        case INDEX_SOIL_EC:
        case INDEX_WATER_LEVEL:
        
        // Advanced parameters
        case INDEX_LIGHT_COLOR_RGB:
        case INDEX_LIGHT_PWM_DUTY:
            return true;
            
        default:
            return false;
    }
}

/**
 * @brief Check if reserved bytes are all zero
 * 
 * @param reserved Pointer to reserved bytes array
 * @return true All bytes are zero
 * @return false At least one byte is non-zero
 */
static bool are_reserved_bytes_zero(const uint8_t* reserved) {
    return (reserved[0] == 0) && (reserved[1] == 0) && (reserved[2] == 0);
}

// ============================================================================
// ID Manipulation Functions Implementation
// ============================================================================

uint16_t greenhouse_build_id(uint8_t func, uint8_t node_id) {
    // Mask inputs to ensure they fit in their bit fields
    func = func & CAN_ID_MASK_FUNC;
    node_id = node_id & CAN_ID_MASK_NODE;
    
    // Build ID: function code in higher bits, node ID in lower bits
    return (func << CAN_ID_FUNC_SHIFT) | node_id;
}

uint8_t greenhouse_get_func_from_id(uint16_t id) {
    // Extract function code from higher bits
    return (id >> CAN_ID_FUNC_SHIFT) & CAN_ID_MASK_FUNC;
}

uint8_t greenhouse_get_node_from_id(uint16_t id) {
    // Extract node ID from lower bits
    return id & CAN_ID_MASK_NODE;
}

// ============================================================================
// Value Conversion Functions Implementation
// ============================================================================

float greenhouse_get_scaling_factor(ParameterIndex index) {
    // Check which parameters need scaling
    switch (index) {
        case INDEX_TEMPERATURE:
        case INDEX_HUMIDITY_AIR:
        case INDEX_HUMIDITY_SOIL:
        case INDEX_SOIL_PH:
            // These parameters use ×100 scaling
            return 100.0f;
            
        case INDEX_LIGHT_PWM_DUTY:
            // PWM duty cycle is 0-100, no scaling needed
            return 1.0f;
            
        default:
            // All other parameters use 1:1 scaling
            return 1.0f;
    }
}

bool greenhouse_needs_scaling(ParameterIndex index) {
    float factor = greenhouse_get_scaling_factor(index);
    return (factor != 1.0f);
}

uint32_t greenhouse_float_to_u32(ParameterIndex index, float value) {
    float factor = greenhouse_get_scaling_factor(index);
    float scaled = value * factor;
    
    // Handle rounding for positive and negative values
    if (scaled >= 0) {
        return (uint32_t)(scaled + 0.5f);
    } else {
        // For negative values (shouldn't happen for most parameters)
        return (uint32_t)(scaled - 0.5f);
    }
}

float greenhouse_u32_to_float(ParameterIndex index, uint32_t raw_value) {
    float factor = greenhouse_get_scaling_factor(index);
    
    if (factor == 0.0f) {
        return 0.0f;  // Should never happen
    }
    
    return (float)raw_value / factor;
}

// ============================================================================
// Packet Parsing and Building Functions Implementation
// ============================================================================

bool greenhouse_parse_packet(uint32_t id, const uint8_t* data, ProtocolResult* result) {
    // Initialize result structure
    if (result == NULL) {
        return false;
    }
    
    memset(result, 0, sizeof(ProtocolResult));
    result->is_valid = false;
    result->is_scaled = false;
    result->param_index = INDEX_INVALID;
    
    // Validate inputs
    if (data == NULL) {
        return false;
    }
    
    // Extract function code and node ID from CAN ID
    uint16_t can_id = id & 0x7FF;  // Mask to 11 bits
    result->func_code = greenhouse_get_func_from_id(can_id);
    result->node_id = greenhouse_get_node_from_id(can_id);
    
    // Validate function code
    if (result->func_code >= FUNC_COUNT) {
        return false;
    }
    
    // Cast data to CAN frame structure for easier access
    const CanDataFrame* frame = (const CanDataFrame*)data;
    
    // Validate CAN frame
    if (!greenhouse_validate_frame(frame)) {
        return false;
    }
    
    // Check if parameter index is valid
    result->param_index = (ParameterIndex)frame->index;
    if (!is_valid_index(result->param_index)) {
        return false;
    }
    
    // Extract raw value (little-endian is already correct for ESP32-S3)
    result->raw_value = frame->value;
    
    // Convert to scaled value if applicable
    if (greenhouse_needs_scaling(result->param_index)) {
        result->scaled_value = greenhouse_u32_to_float(result->param_index, result->raw_value);
        result->is_scaled = true;
    } else {
        // For parameters without scaling, use raw value as float
        result->scaled_value = (float)result->raw_value;
        result->is_scaled = false;
    }
    
    // Mark as valid
    result->is_valid = true;
    return true;
}

void greenhouse_build_packet(uint8_t* buffer, ParameterIndex index, float value) {
    if (buffer == NULL) {
        return;
    }
    
    // Convert float to raw u32
    uint32_t raw_value = greenhouse_float_to_u32(index, value);
    
    // Build packet with raw value
    greenhouse_build_packet_raw(buffer, index, raw_value);
}

void greenhouse_build_packet_raw(uint8_t* buffer, ParameterIndex index, uint32_t raw_value) {
    if (buffer == NULL) {
        return;
    }
    
    // Cast buffer to CAN frame structure for easier filling
    CanDataFrame* frame = (CanDataFrame*)buffer;
    
    // Fill the frame
    frame->index = (uint8_t)index;
    frame->reserved[0] = 0;
    frame->reserved[1] = 0;
    frame->reserved[2] = 0;
    frame->value = raw_value;  // Little-endian is native for ESP32-S3
}

bool greenhouse_validate_frame(const CanDataFrame* frame) {
    if (frame == NULL) {
        return false;
    }
    
    // Check reserved bytes
    if (!are_reserved_bytes_zero(frame->reserved)) {
        return false;
    }
    
    // Check if index is valid
    if (!is_valid_index((ParameterIndex)frame->index)) {
        return false;
    }
    
    return true;
}

// ============================================================================
// Utility Functions Implementation
// ============================================================================

const char* greenhouse_get_param_name(ParameterIndex index) {
    switch (index) {
        // System parameters
        case INDEX_SYSTEM_TIMESTAMP:    return "System Timestamp";
        case INDEX_HEARTBEAT:           return "Heartbeat";
        case INDEX_ERROR_CODE:          return "Error Code";
        case INDEX_CONTROL_MODE:        return "Control Mode";
        
        // Switch parameters
        case INDEX_LIGHT_MAIN_POWER:    return "Light Main Power";
        case INDEX_WATER_PUMP:          return "Water Pump";
        case INDEX_HUMIDIFIER:          return "Humidifier";
        case INDEX_VENTILATION_FAN:     return "Ventilation Fan";
        case INDEX_SUNSHADE_MOTOR:      return "Sunshade Motor";
        case INDEX_HEATER:              return "Heater";
        case INDEX_WINDOW_ACTUATOR:     return "Window Actuator";
        
        // Analog values
        case INDEX_TEMPERATURE:         return "Temperature";
        case INDEX_HUMIDITY_AIR:        return "Air Humidity";
        case INDEX_HUMIDITY_SOIL:       return "Soil Humidity";
        case INDEX_LIGHT_INTENSITY:     return "Light Intensity";
        case INDEX_CO2_LEVEL:           return "CO2 Level";
        case INDEX_FAN_SPEED:           return "Fan Speed";
        case INDEX_SOIL_PH:             return "Soil pH";
        case INDEX_SOIL_EC:             return "Soil EC";
        case INDEX_WATER_LEVEL:         return "Water Level";
        
        // Advanced parameters
        case INDEX_LIGHT_COLOR_RGB:     return "Light Color RGB";
        case INDEX_LIGHT_PWM_DUTY:      return "Light PWM Duty";
        
        default:                        return "Unknown Parameter";
    }
}

const char* greenhouse_get_func_name(uint8_t func) {
    switch (func) {
        case FUNC_ALERT:     return "Alert";
        case FUNC_TIME_SYNC: return "Time Sync";
        case FUNC_WRITE_SET: return "Write/Set";
        case FUNC_REPORT:    return "Report";
        default:             return "Unknown Function";
    }
}