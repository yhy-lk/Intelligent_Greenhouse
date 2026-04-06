/**
 * @file greenhouse_protocol.h
 * @brief Greenhouse CAN Bus Protocol Definitions and Utilities
 * 
 * This module implements the CAN bus communication protocol for greenhouse automation systems.
 * It provides functions for packing/unpacking CAN messages, converting between physical
 * values and raw CAN data, and handling the protocol's specific data formats.
 * 
 * Protocol features:
 * - 11-bit CAN IDs with 4-bit function code + 7-bit node ID
 * - 8-byte data frames with 4-byte alignment
 * - Little-endian byte order (ESP32-S3 architecture)
 * - Parameter index based addressing with scaling factors
 * 
 * @note The implementation assumes little-endian architecture (ESP32-S3).
 */

#ifndef GREENHOUSE_PROTOCOL_H
#define GREENHOUSE_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Protocol Constants
// ============================================================================

/** CAN ID bit allocation */
#define CAN_ID_FUNC_BITS       4
#define CAN_ID_NODE_BITS       7
#define CAN_ID_FUNC_SHIFT      CAN_ID_NODE_BITS
#define CAN_ID_MASK_FUNC       ((1 << CAN_ID_FUNC_BITS) - 1)
#define CAN_ID_MASK_NODE       ((1 << CAN_ID_NODE_BITS) - 1)

/** Maximum node ID */
#define MAX_NODE_ID            ((1 << CAN_ID_NODE_BITS) - 1)

/** Maximum function code */
#define MAX_FUNC_CODE          ((1 << CAN_ID_FUNC_BITS) - 1)

/** CAN data frame size */
#define CAN_DATA_SIZE          8

// ============================================================================
// Function Code Definitions (4 bits)
// ============================================================================

/**
 * @brief Function codes for CAN messages
 */
typedef enum {
    FUNC_ALERT     = 0x0,  ///< Alert message (highest priority)
    FUNC_TIME_SYNC = 0x1,  ///< Time synchronization
    FUNC_WRITE_SET = 0x2,  ///< Host write/set command
     FUNC_REPORT    = 0x3,  ///< Slave report/current data
    
    FUNC_COUNT     = 4     ///< Total number of function codes
} FunctionCode;

// ============================================================================
// Parameter Index Definitions
// ============================================================================

/**
 * @brief Parameter indices for greenhouse control system
 */
typedef enum {
    // System parameters
    INDEX_SYSTEM_TIMESTAMP    = 0x00,  ///< Unix timestamp (seconds, u32)
    INDEX_HEARTBEAT           = 0x01,  ///< Uptime (seconds, u32)
    INDEX_ERROR_CODE          = 0x02,  ///< Error bitmask (u32)
    INDEX_CONTROL_MODE        = 0x03,  ///< 0=Manual, 1=Auto (u32)
    
    // Switch parameters (0=OFF, 1=ON)
    INDEX_LIGHT_MAIN_POWER    = 0x10,  ///< Main light power
    INDEX_WATER_PUMP          = 0x11,  ///< Water pump
    INDEX_HUMIDIFIER          = 0x12,  ///< Humidifier
    INDEX_VENTILATION_FAN     = 0x13,  ///< Ventilation fan switch
    INDEX_SUNSHADE_MOTOR      = 0x14,  ///< Sunshade motor
    INDEX_HEATER              = 0x15,  ///< Heater
    INDEX_WINDOW_ACTUATOR     = 0x16,  ///< Window actuator
    
    // Analog values (scaled u32)
    INDEX_TEMPERATURE         = 0x30,  ///< Temperature (°C × 100)
    INDEX_HUMIDITY_AIR        = 0x31,  ///< Air humidity (% × 100)
    INDEX_HUMIDITY_SOIL       = 0x32,  ///< Soil humidity (% × 100)
    INDEX_LIGHT_INTENSITY     = 0x33,  ///< Light intensity (Lux × 1)
    INDEX_CO2_LEVEL           = 0x34,  ///< CO2 level (ppm × 1)
    INDEX_FAN_SPEED           = 0x35,  ///< Fan speed (RPM × 1)
    INDEX_SOIL_PH             = 0x36,  ///< Soil pH (pH × 100)
    INDEX_SOIL_EC             = 0x37,  ///< Soil EC (μS/cm × 1)
    INDEX_WATER_LEVEL         = 0x38,  ///< Water level (% × 1)
    
    // Advanced parameters
    INDEX_LIGHT_COLOR_RGB     = 0x50,  ///< Light color (0x00RRGGBB)
    INDEX_LIGHT_PWM_DUTY      = 0x51,  ///< Light PWM duty cycle (0-100)
    
    /** Special value indicating invalid parameter */
    INDEX_INVALID             = 0xFF
} ParameterIndex;

// ============================================================================
// Data Structures
// ============================================================================

/**
 * @brief CAN data frame structure (packed for exact byte alignment)
 * 
 * Byte layout:
 * - Byte 0: Parameter index
 * - Bytes 1-3: Reserved (0x00)
 * - Bytes 4-7: Data value (u32, little-endian)
 */
typedef struct __attribute__((packed)) {
    uint8_t  index;       ///< Parameter index
    uint8_t  reserved[3]; ///< Reserved bytes (must be 0x00)
    uint32_t value;       ///< Data value (little-endian)
} CanDataFrame;

/**
 * @brief Result of parsing a CAN packet
 */
typedef struct {
    uint8_t       func_code;      ///< Extracted function code
    uint8_t       node_id;        ///< Extracted node ID
    ParameterIndex param_index;   ///< Parameter index
    uint32_t      raw_value;      ///< Raw u32 value from CAN frame
    float         scaled_value;   ///< Scaled physical value (if applicable)
    bool          is_scaled;      ///< Whether scaled_value is valid
    bool          is_valid;       ///< Whether the packet was parsed successfully
} ProtocolResult;

// ============================================================================
// ID Manipulation Functions
// ============================================================================

/**
 * @brief Build CAN ID from function code and node ID
 * 
 * @param func Function code (4 bits)
 * @param node_id Node ID (7 bits)
 * @return uint16_t 11-bit CAN ID
 */
uint16_t greenhouse_build_id(uint8_t func, uint8_t node_id);

/**
 * @brief Extract function code from CAN ID
 * 
 * @param id 11-bit CAN ID
 * @return uint8_t Function code (4 bits)
 */
uint8_t greenhouse_get_func_from_id(uint16_t id);

/**
 * @brief Extract node ID from CAN ID
 * 
 * @param id 11-bit CAN ID
 * @return uint8_t Node ID (7 bits)
 */
uint8_t greenhouse_get_node_from_id(uint16_t id);

// ============================================================================
// Value Conversion Functions
// ============================================================================

/**
 * @brief Convert physical value to raw u32 for CAN transmission
 * 
 * @param index Parameter index
 * @param value Physical value (float)
 * @return uint32_t Raw u32 value for CAN frame
 * 
 * @note Automatically applies scaling factor based on parameter index
 */
uint32_t greenhouse_float_to_u32(ParameterIndex index, float value);

/**
 * @brief Convert raw u32 from CAN frame to physical value
 * 
 * @param index Parameter index
 * @param raw_value Raw u32 value from CAN frame
 * @return float Physical value
 * 
 * @note Automatically applies inverse scaling factor based on parameter index
 */
float greenhouse_u32_to_float(ParameterIndex index, uint32_t raw_value);

/**
 * @brief Get scaling factor for a parameter index
 * 
 * @param index Parameter index
 * @return float Scaling factor (multiply physical value by this to get raw u32)
 */
float greenhouse_get_scaling_factor(ParameterIndex index);

/**
 * @brief Check if a parameter index requires scaling
 * 
 * @param index Parameter index
 * @return true Parameter requires scaling (×100)
 * @return false Parameter uses 1:1 scaling
 */
bool greenhouse_needs_scaling(ParameterIndex index);

// ============================================================================
// Packet Parsing and Building Functions
// ============================================================================

/**
 * @brief Parse a received CAN packet
 * 
 * @param id 11-bit CAN ID
 * @param data Pointer to 8-byte CAN data
 * @param result Pointer to ProtocolResult structure to fill
 * @return true Packet parsed successfully
 * @return false Packet parsing failed (invalid format, reserved bytes not zero, etc.)
 */
bool greenhouse_parse_packet(uint32_t id, const uint8_t* data, ProtocolResult* result);

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
void greenhouse_build_packet(uint8_t* buffer, ParameterIndex index, float value);

/**
 * @brief Build CAN data frame with raw u32 value
 * 
 * @param buffer Pointer to 8-byte buffer to fill
 * @param index Parameter index
 * @param raw_value Raw u32 value (already scaled)
 */
void greenhouse_build_packet_raw(uint8_t* buffer, ParameterIndex index, uint32_t raw_value);

/**
 * @brief Validate CAN data frame structure
 * 
 * @param frame Pointer to CAN data frame
 * @return true Frame is valid (reserved bytes are zero, index is valid)
 * @return false Frame is invalid
 */
bool greenhouse_validate_frame(const CanDataFrame* frame);

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * @brief Get parameter name as string (for debugging)
 * 
 * @param index Parameter index
 * @return const char* Parameter name string
 */
const char* greenhouse_get_param_name(ParameterIndex index);

/**
 * @brief Get function code name as string (for debugging)
 * 
 * @param func Function code
 * @return const char* Function name string
 */
const char* greenhouse_get_func_name(uint8_t func);

#ifdef __cplusplus
}
#endif

#endif // GREENHOUSE_PROTOCOL_H