/**
 * @file sensor_state.h
 * @brief Smart Greenhouse Digital Twin - Middleware Service Layer
 * 
 * This module implements a digital twin service layer for smart greenhouse systems.
 * It maintains both current sensor readings and target settings for all nodes,
 * providing a clean abstraction between CAN protocol layer and JSON/AI logic layer.
 * 
 * Architecture positioning:
 * - Above: CAN Protocol Layer (raw CAN data)
 * - Below: JSON/AI Logic Layer (standardized API for data access)
 * 
 * Key features:
 * - Maintains current state and target settings for all nodes (digital twin)
 * - No dependency on ArduinoJson or specific hardware drivers
 * - Pure C++ implementation using standard libraries only
 * - Memory-optimized using fixed arrays (space-for-time tradeoff)
 * - Thread-safe for single-threaded ESP32-S3 applications
 */

#ifndef SENSOR_STATE_H
#define SENSOR_STATE_H

#include <stdint.h>
#include <stdbool.h>

// Reuse CAN protocol parameter index definitions
#include "../CAN/internal_can_protocol.h"

namespace Greenhouse {

// ============================================================================
// Configuration Constants
// ============================================================================

/** Maximum number of nodes in the system (NodeID 0-15) */
constexpr uint8_t MAX_NODES = 16;

/**
 * @brief Maximum parameter index value for array sizing
 * 
 * Parameter indices range from 0x00 to 0x51 (0x51 = INDEX_LIGHT_PWM_DUTY)
 * Using 0x60 (96) provides safe padding for future expansion while
 * keeping memory usage reasonable on ESP32-S3.
 */
constexpr uint8_t MAX_PARAM_INDEX = 0x60;

/** Special value indicating invalid/uninitialized data */
constexpr float INVALID_VALUE = -9999.0f;

/** Default node timeout in milliseconds (5 minutes) */
constexpr uint32_t DEFAULT_NODE_TIMEOUT_MS = 5 * 60 * 1000;

// ============================================================================
// Data Structures
// ============================================================================

/**
 * @brief Digital twin data for a single node
 * 
 * This structure maintains both current sensor readings and target settings
 * for all parameters of a single greenhouse node (device).
 * 
 * Digital twin concept:
 * - current_values: Actual sensor readings from the physical device
 * - target_values: Desired settings sent by user or AI controller
 */
struct NodeData {
    bool is_online;                    ///< Whether the node is currently online
    uint32_t last_seen_timestamp;      ///< Last communication timestamp (ms)
    
    /**
     * @brief Current sensor readings (actual values from physical device)
     * 
     * Array indexed by parameter index (0x00 to MAX_PARAM_INDEX-1).
     * Values are stored as floats for easy processing by business logic.
     * Switch values are stored as 0.0f (OFF) or 1.0f (ON).
     * 
     * @note Uses INVALID_VALUE for uninitialized/unavailable parameters.
     */
    float current_values[MAX_PARAM_INDEX];
    
    /**
     * @brief Target settings (desired values set by user/AI)
     * 
     * Array indexed by parameter index (0x00 to MAX_PARAM_INDEX-1).
     * These represent the desired state that the system should achieve.
     * 
     * @note Uses INVALID_VALUE for parameters without target settings.
     */
    float target_values[MAX_PARAM_INDEX];
    
    /**
     * @brief Default constructor initializes all values to invalid
     */
    NodeData() : is_online(false), last_seen_timestamp(0) {
        for (uint16_t i = 0; i < MAX_PARAM_INDEX; i++) {
            current_values[i] = INVALID_VALUE;
            target_values[i] = INVALID_VALUE;
        }
    }
};

// ============================================================================
// SensorState Class
// ============================================================================

/**
 * @class SensorState
 * @brief Digital twin service layer for smart greenhouse system
 * 
 * This class implements the middleware service layer that maintains a complete
 * digital twin of the greenhouse system. It stores both current sensor readings
 * and target settings for all nodes, providing a clean API for upper layers.
 * 
 * Key responsibilities:
 * 1. Ingest data from CAN protocol layer (current readings and target acknowledgments)
 * 2. Maintain node online/offline status
 * 3. Provide read-only access to current and target values
 * 4. Support error detection (deviations between current and target)
 * 
 * Design principles:
 * - No dynamic memory allocation (fixed arrays)
 * - Simple, fast access patterns
 * - Separation of concerns (only data storage, no business logic)
 * - Thread-safe for single-threaded operation
 */
class SensorState {
private:
    /** Array of node data for all possible nodes (0-15) */
    NodeData nodes_[MAX_NODES];
    
    /** Current system timestamp (ms) - should be updated regularly */
    uint32_t current_timestamp_;
    
    /** Node timeout threshold (ms) */
    uint32_t node_timeout_ms_;
    
    /**
     * @brief Validate node ID
     * 
     * @param node_id Node ID to validate
     * @return true Node ID is valid (0-15)
     * @return false Node ID is invalid
     */
    bool isValidNodeId(uint8_t node_id) const {
        return node_id < MAX_NODES;
    }
    
    /**
     * @brief Validate parameter index
     * 
     * @param param_index Parameter index to validate
     * @return true Parameter index is valid (0x00 to MAX_PARAM_INDEX-1)
     * @return false Parameter index is invalid
     */
    bool isValidParamIndex(uint8_t param_index) const {
        return param_index < MAX_PARAM_INDEX;
    }
    
public:
    /**
     * @brief Default constructor
     * 
     * @param timeout_ms Node timeout in milliseconds (default: 5 minutes)
     */
    explicit SensorState(uint32_t timeout_ms = DEFAULT_NODE_TIMEOUT_MS);
    
    /**
     * @brief Destructor
     */
    ~SensorState() = default;
    
    // ========================================================================
    // Data Ingestion API (from CAN Protocol Layer)
    // ========================================================================
    
    /**
     * @brief Update node state with new data from CAN protocol layer
     * 
     * This method is called when the CAN protocol layer parses a valid
     * CAN message. It updates either current sensor readings or target
     * settings based on the message type.
     * 
     * @param node_id Node ID (0-15) that sent the data
     * @param param_index Parameter index (e.g., 0x30 for temperature)
     * @param value Physical value as float (already scaled)
     * @param is_target true: This is a target setting acknowledgment from node
     *                  false: This is a current sensor reading from node
     * 
     * @note The value should already be converted to physical units (e.g., °C)
     *       by the CAN protocol layer before calling this method.
     * 
     * @return true Update successful
     * @return false Update failed (invalid node_id or param_index)
     */
    bool updateState(uint8_t node_id, uint8_t param_index, float value, bool is_target);
    
    /**
     * @brief Update current system timestamp
     * 
     * This should be called regularly (e.g., from main loop) to keep
     * node online/offline detection accurate.
     * 
     * @param timestamp Current timestamp in milliseconds
     */
    void updateTimestamp(uint32_t timestamp) {
        current_timestamp_ = timestamp;
    }
    
    // ========================================================================
    // Node Management API
    // ========================================================================
    
    /**
     * @brief Mark a node as online (received communication)
     * 
     * @param node_id Node ID to mark as online (0-15)
     * @return true Success
     * @return false Invalid node ID
     */
    bool markNodeOnline(uint8_t node_id);
    
    /**
     * @brief Check if a node is currently online
     * 
     * A node is considered online if it has communicated within the
     * configured timeout period.
     * 
     * @param node_id Node ID to check (0-15)
     * @return true Node is online
     * @return false Node is offline or invalid node ID
     */
    bool isNodeOnline(uint8_t node_id) const;
    
    /**
     * @brief Get the last seen timestamp for a node
     * 
     * @param node_id Node ID (0-15)
     * @return uint32_t Last seen timestamp in milliseconds, 0 if invalid node
     */
    uint32_t getNodeLastSeen(uint8_t node_id) const;
    
    /**
     * @brief Set node timeout threshold
     * 
     * @param timeout_ms Timeout in milliseconds
     */
    void setNodeTimeout(uint32_t timeout_ms) {
        node_timeout_ms_ = timeout_ms;
    }
    
    /**
     * @brief Get node timeout threshold
     * 
     * @return uint32_t Timeout in milliseconds
     */
    uint32_t getNodeTimeout() const {
        return node_timeout_ms_;
    }
    
    // ========================================================================
    // Data Access API (for JSON/AI Logic Layer)
    // ========================================================================
    
    /**
     * @brief Get current sensor value for a node and parameter
     * 
     * @param node_id Node ID (0-15)
     * @param param_index Parameter index (e.g., 0x30 for temperature)
     * @return float Current sensor value, or INVALID_VALUE if unavailable/invalid
     */
    float getCurrentValue(uint8_t node_id, uint8_t param_index) const;
    
    /**
     * @brief Get target setting value for a node and parameter
     * 
     * @param node_id Node ID (0-15)
     * @param param_index Parameter index (e.g., 0x30 for temperature)
     * @return float Target setting value, or INVALID_VALUE if unavailable/invalid
     */
    float getTargetValue(uint8_t node_id, uint8_t param_index) const;
    
    /**
     * @brief Get direct access to node data (for advanced use cases)
     * 
     * @param node_id Node ID (0-15)
     * @return const NodeData* Pointer to node data, or nullptr if invalid node ID
     */
    const NodeData* getNodeData(uint8_t node_id) const;
    
    /**
     * @brief Check if a current value is valid (not INVALID_VALUE)
     * 
     * @param node_id Node ID (0-15)
     * @param param_index Parameter index
     * @return true Value is valid
     * @return false Value is invalid/unavailable
     */
    bool hasCurrentValue(uint8_t node_id, uint8_t param_index) const;
    
    /**
     * @brief Check if a target value is valid (not INVALID_VALUE)
     * 
     * @param node_id Node ID (0-15)
     * @param param_index Parameter index
     * @return true Value is valid
     * @return false Value is invalid/unavailable
     */
    bool hasTargetValue(uint8_t node_id, uint8_t param_index) const;
    
    // ========================================================================
    // Business Logic Helper API
    // ========================================================================
    
    /**
     * @brief Check if a node has errors
     * 
     * A node is considered to have errors if:
     * 1. It's offline (no recent communication), OR
     * 2. Any critical parameter has excessive deviation between current and target
     * 
     * @param node_id Node ID to check (0-15)
     * @return true Node has errors
     * @return false Node is operating normally
     */
    bool hasError(uint8_t node_id) const;
    
    /**
     * @brief Check if a specific parameter has excessive deviation
     * 
     * @param node_id Node ID (0-15)
     * @param param_index Parameter index
     * @param max_deviation Maximum allowed deviation
     * @return true Deviation exceeds maximum allowed
     * @return false Deviation is within limits or data unavailable
     */
    bool hasExcessiveDeviation(uint8_t node_id, uint8_t param_index, float max_deviation) const;
    
    /**
     * @brief Reset all data for a specific node
     * 
     * @param node_id Node ID to reset (0-15)
     * @return true Success
     * @return false Invalid node ID
     */
    bool resetNode(uint8_t node_id);
    
    /**
     * @brief Reset all nodes (factory reset)
     */
    void resetAll();
    
    // ========================================================================
    // Diagnostic and Debug API
    // ========================================================================
    
    /**
     * @brief Get number of online nodes
     * 
     * @return uint8_t Count of nodes currently online
     */
    uint8_t getOnlineNodeCount() const;
    
    /**
     * @brief Get total number of valid current readings across all nodes
     * 
     * @return uint16_t Count of valid current readings
     */
    uint16_t getValidCurrentReadingCount() const;
    
    /**
     * @brief Get total number of valid target settings across all nodes
     * 
     * @return uint16_t Count of valid target settings
     */
    uint16_t getValidTargetSettingCount() const;
};

} // namespace Greenhouse

#endif // SENSOR_STATE_H