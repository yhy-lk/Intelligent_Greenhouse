/**
 * @file GreenhouseProtocol.h
 * @brief C++ Wrapper Class for Greenhouse CAN Bus Protocol
 * 
 * This class provides a C++ interface for the greenhouse CAN bus communication protocol.
 * It wraps the C functions defined in greenhouse_protocol.h and provides additional
 * convenience methods for easier use in C++ code.
 * 
 * Protocol features:
 * - 11-bit CAN IDs with 4-bit function code + 7-bit node ID
 * - 8-byte data frames with 4-byte alignment
 * - Little-endian byte order (ESP32-S3 architecture)
 * - Parameter index based addressing with scaling factors
 */

#ifndef GREENHOUSE_PROTOCOL_CLASS_H
#define GREENHOUSE_PROTOCOL_CLASS_H

#include <stdint.h>
#include <stdbool.h>
#include "internal_can_protocol.h"

/**
 * @class GreenhouseProtocol
 * @brief C++ wrapper class for greenhouse CAN bus protocol
 */
class GreenhouseProtocol {
public:
    /**
     * @brief Default constructor
     */
    GreenhouseProtocol() = default;
    
    /**
     * @brief Destructor
     */
    ~GreenhouseProtocol() = default;
    
    // ========================================================================
    // ID Manipulation Functions
    // ========================================================================
    
    /**
     * @brief Build CAN ID from function code and node ID
     * 
     * @param func Function code (4 bits)
     * @param node_id Node ID (7 bits)
     * @return uint16_t 11-bit CAN ID
     */
    static uint16_t buildID(uint8_t func, uint8_t node_id) {
        return greenhouse_build_id(func, node_id);
    }
    
    /**
     * @brief Extract function code from CAN ID
     * 
     * @param id 11-bit CAN ID
     * @return uint8_t Function code (4 bits)
     */
    static uint8_t getFuncFromID(uint16_t id) {
        return greenhouse_get_func_from_id(id);
    }
    
    /**
     * @brief Extract node ID from CAN ID
     * 
     * @param id 11-bit CAN ID
     * @return uint8_t Node ID (7 bits)
     */
    static uint8_t getNodeFromID(uint16_t id) {
        return greenhouse_get_node_from_id(id);
    }
    
    // ========================================================================
    // Value Conversion Functions
    // ========================================================================
    
    /**
     * @brief Convert physical value to raw u32 for CAN transmission
     * 
     * @param index Parameter index
     * @param value Physical value (float)
     * @return uint32_t Raw u32 value for CAN frame
     * 
     * @note Automatically applies scaling factor based on parameter index
     */
    static uint32_t floatToU32(ParameterIndex index, float value) {
        return greenhouse_float_to_u32(index, value);
    }
    
    /**
     * @brief Convert raw u32 from CAN frame to physical value
     * 
     * @param index Parameter index
     * @param raw_value Raw u32 value from CAN frame
     * @return float Physical value
     * 
     * @note Automatically applies inverse scaling factor based on parameter index
     */
    static float u32ToFloat(ParameterIndex index, uint32_t raw_value) {
        return greenhouse_u32_to_float(index, raw_value);
    }
    
    /**
     * @brief Get scaling factor for a parameter index
     * 
     * @param index Parameter index
     * @return float Scaling factor (multiply physical value by this to get raw u32)
     */
    static float getScalingFactor(ParameterIndex index) {
        return greenhouse_get_scaling_factor(index);
    }
    
    /**
     * @brief Check if a parameter index requires scaling
     * 
     * @param index Parameter index
     * @return true Parameter requires scaling (×100)
     * @return false Parameter uses 1:1 scaling
     */
    static bool needsScaling(ParameterIndex index) {
        return greenhouse_needs_scaling(index);
    }
    
    // ========================================================================
    // Packet Parsing and Building Functions
    // ========================================================================
    
    /**
     * @brief Parse a received CAN packet
     * 
     * @param id 11-bit CAN ID
     * @param data Pointer to 8-byte CAN data
     * @param result Reference to ProtocolResult structure to fill
     * @return true Packet parsed successfully
     * @return false Packet parsing failed (invalid format, reserved bytes not zero, etc.)
     */
    static bool parsePacket(uint32_t id, const uint8_t* data, ProtocolResult& result) {
        return greenhouse_parse_packet(id, data, &result);
    }
    
    /**
     * @brief Build CAN data frame for transmission
     * 
     * @param buffer Pointer to 8-byte buffer to fill
     * @param index Parameter index
     * @param value Physical value (float)
     * 
     * @note The buffer will be filled with the proper CAN data frame format.
     *       The caller is responsible for providing an 8-byte buffer.
     */
    static void buildPacket(uint8_t* buffer, ParameterIndex index, float value) {
        greenhouse_build_packet(buffer, index, value);
    }
    
    /**
     * @brief Build CAN data frame with raw u32 value
     * 
     * @param buffer Pointer to 8-byte buffer to fill
     * @param index Parameter index
     * @param raw_value Raw u32 value (already scaled)
     */
    static void buildPacketRaw(uint8_t* buffer, ParameterIndex index, uint32_t raw_value) {
        greenhouse_build_packet_raw(buffer, index, raw_value);
    }
    
    /**
     * @brief Validate CAN data frame structure
     * 
     * @param frame Pointer to CAN data frame
     * @return true Frame is valid (reserved bytes are zero, index is valid)
     * @return false Frame is invalid
     */
    static bool validateFrame(const CanDataFrame* frame) {
        return greenhouse_validate_frame(frame);
    }
    
    /**
     * @brief Validate CAN data frame structure from byte buffer
     * 
     * @param data Pointer to 8-byte CAN data buffer
     * @return true Frame is valid
     * @return false Frame is invalid
     */
    static bool validateFrame(const uint8_t* data) {
        if (data == nullptr) {
            return false;
        }
        const CanDataFrame* frame = reinterpret_cast<const CanDataFrame*>(data);
        return greenhouse_validate_frame(frame);
    }
    
    // ========================================================================
    // Utility Functions
    // ========================================================================
    
    /**
     * @brief Get parameter name as string (for debugging)
     * 
     * @param index Parameter index
     * @return const char* Parameter name string
     */
    static const char* getParamName(ParameterIndex index) {
        return greenhouse_get_param_name(index);
    }
    
    /**
     * @brief Get function code name as string (for debugging)
     * 
     * @param func Function code
     * @return const char* Function name string
     */
    static const char* getFuncName(uint8_t func) {
        return greenhouse_get_func_name(func);
    }
    
    /**
     * @brief Get CAN data frame size
     * 
     * @return size_t Size of CAN data frame in bytes
     */
    static constexpr size_t getDataFrameSize() {
        return CAN_DATA_SIZE;
    }
    
    /**
     * @brief Get maximum node ID
     * 
     * @return uint8_t Maximum node ID value
     */
    static constexpr uint8_t getMaxNodeID() {
        return MAX_NODE_ID;
    }
    
    /**
     * @brief Get maximum function code
     * 
     * @return uint8_t Maximum function code value
     */
    static constexpr uint8_t getMaxFuncCode() {
        return MAX_FUNC_CODE;
    }
    
    /**
     * @brief Check if a parameter index is valid
     * 
     * @param index Parameter index to check
     * @return true Index is valid
     * @return false Index is invalid
     */
    static bool isValidIndex(ParameterIndex index) {
        // Reuse the validation logic from the C implementation
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
     * @brief Create a ProtocolResult structure with default values
     * 
     * @return ProtocolResult Initialized result structure
     */
    static ProtocolResult createResult() {
        ProtocolResult result;
        result.func_code = 0;
        result.node_id = 0;
        result.param_index = INDEX_INVALID;
        result.raw_value = 0;
        result.scaled_value = 0.0f;
        result.is_scaled = false;
        result.is_valid = false;
        return result;
    }
};

#endif // GREENHOUSE_PROTOCOL_CLASS_H