/**
 * @file sensor_state.cpp
 * @brief Implementation of Smart Greenhouse Digital Twin Service Layer
 * 
 * This file implements the SensorState class which provides a middleware
 * service layer for maintaining digital twin state of greenhouse nodes.
 */

#include "sensor_state.h"
#include <string.h>

namespace Greenhouse {

// ============================================================================
// SensorState Implementation
// ============================================================================

SensorState::SensorState(uint32_t timeout_ms) 
    : current_timestamp_(0), node_timeout_ms_(timeout_ms) {
    // Constructor automatically initializes all nodes through NodeData default constructor
    // No additional initialization needed
}

bool SensorState::updateState(uint8_t node_id, uint8_t param_index, float value, bool is_target) {
    // Validate inputs
    if (!isValidNodeId(node_id) || !isValidParamIndex(param_index)) {
        return false;
    }
    
    // Get reference to node data
    NodeData& node = nodes_[node_id];
    
    // Update the appropriate value array
    if (is_target) {
        node.target_values[param_index] = value;
    } else {
        node.current_values[param_index] = value;
    }
    
    // Mark node as online and update timestamp
    node.is_online = true;
    node.last_seen_timestamp = current_timestamp_;
    
    return true;
}

bool SensorState::markNodeOnline(uint8_t node_id) {
    if (!isValidNodeId(node_id)) {
        return false;
    }
    
    NodeData& node = nodes_[node_id];
    node.is_online = true;
    node.last_seen_timestamp = current_timestamp_;
    
    return true;
}

bool SensorState::isNodeOnline(uint8_t node_id) const {
    if (!isValidNodeId(node_id)) {
        return false;
    }
    
    const NodeData& node = nodes_[node_id];
    
    // Node is online if it has been marked online AND
    // we haven't timed out since last communication
    if (!node.is_online) {
        return false;
    }
    
    // Check for timeout (if current timestamp is available)
    if (current_timestamp_ > 0 && node_timeout_ms_ > 0) {
        uint32_t time_since_last_seen = current_timestamp_ - node.last_seen_timestamp;
        if (time_since_last_seen > node_timeout_ms_) {
            // Node has timed out
            const_cast<NodeData&>(node).is_online = false;  // Safe cast for timeout update
            return false;
        }
    }
    
    return true;
}

uint32_t SensorState::getNodeLastSeen(uint8_t node_id) const {
    if (!isValidNodeId(node_id)) {
        return 0;
    }
    
    return nodes_[node_id].last_seen_timestamp;
}

float SensorState::getCurrentValue(uint8_t node_id, uint8_t param_index) const {
    if (!isValidNodeId(node_id) || !isValidParamIndex(param_index)) {
        return INVALID_VALUE;
    }
    
    return nodes_[node_id].current_values[param_index];
}

float SensorState::getTargetValue(uint8_t node_id, uint8_t param_index) const {
    if (!isValidNodeId(node_id) || !isValidParamIndex(param_index)) {
        return INVALID_VALUE;
    }
    
    return nodes_[node_id].target_values[param_index];
}

const NodeData* SensorState::getNodeData(uint8_t node_id) const {
    if (!isValidNodeId(node_id)) {
        return nullptr;
    }
    
    return &nodes_[node_id];
}

bool SensorState::hasCurrentValue(uint8_t node_id, uint8_t param_index) const {
    if (!isValidNodeId(node_id) || !isValidParamIndex(param_index)) {
        return false;
    }
    
    float value = nodes_[node_id].current_values[param_index];
    return (value != INVALID_VALUE);
}

bool SensorState::hasTargetValue(uint8_t node_id, uint8_t param_index) const {
    if (!isValidNodeId(node_id) || !isValidParamIndex(param_index)) {
        return false;
    }
    
    float value = nodes_[node_id].target_values[param_index];
    return (value != INVALID_VALUE);
}

bool SensorState::hasError(uint8_t node_id) const {
    if (!isValidNodeId(node_id)) {
        return true;  // Invalid node ID is considered an error
    }
    
    // Check if node is offline
    if (!isNodeOnline(node_id)) {
        return true;
    }
    
    // Define critical parameters that should be checked for deviations
    // These are parameters that have both current readings and target settings
    const uint8_t critical_params[] = {
        INDEX_TEMPERATURE,      // Temperature
        INDEX_HUMIDITY_AIR,     // Air humidity
        INDEX_HUMIDITY_SOIL,    // Soil humidity
        INDEX_LIGHT_INTENSITY,  // Light intensity
        INDEX_CO2_LEVEL,        // CO2 level
        INDEX_SOIL_PH,          // Soil pH
        INDEX_SOIL_EC,          // Soil EC
        INDEX_WATER_LEVEL,      // Water level
        INDEX_LIGHT_PWM_DUTY    // Light PWM duty
    };
    
    const uint8_t num_critical_params = sizeof(critical_params) / sizeof(critical_params[0]);
    
    // Define maximum allowed deviations for each parameter type
    const float max_deviations[] = {
        2.0f,   // Temperature (°C) - ±2°C tolerance
        5.0f,   // Humidity (%) - ±5% tolerance
        5.0f,   // Soil humidity (%) - ±5% tolerance
        50.0f,  // Light intensity (Lux) - ±50 Lux tolerance
        50.0f,  // CO2 level (ppm) - ±50 ppm tolerance
        0.5f,   // Soil pH - ±0.5 pH tolerance
        50.0f,  // Soil EC (μS/cm) - ±50 μS/cm tolerance
        10.0f,  // Water level (%) - ±10% tolerance
        5.0f    // Light PWM duty (%) - ±5% tolerance
    };
    
    // Check each critical parameter for excessive deviation
    for (uint8_t i = 0; i < num_critical_params; i++) {
        uint8_t param_index = critical_params[i];
        float max_deviation = max_deviations[i];
        
        if (hasExcessiveDeviation(node_id, param_index, max_deviation)) {
            return true;  // Found a parameter with excessive deviation
        }
    }
    
    return false;  // No errors detected
}

bool SensorState::hasExcessiveDeviation(uint8_t node_id, uint8_t param_index, float max_deviation) const {
    if (!isValidNodeId(node_id) || !isValidParamIndex(param_index)) {
        return false;  // Not considered excessive deviation
    }
    
    const NodeData& node = nodes_[node_id];
    float current = node.current_values[param_index];
    float target = node.target_values[param_index];
    
    // Check if both values are valid
    if (current == INVALID_VALUE || target == INVALID_VALUE) {
        return false;  // Missing data, can't determine deviation
    }
    
    // Calculate absolute deviation
    float deviation = (current > target) ? (current - target) : (target - current);
    
    // Return true if deviation exceeds maximum allowed
    return (deviation > max_deviation);
}

bool SensorState::resetNode(uint8_t node_id) {
    if (!isValidNodeId(node_id)) {
        return false;
    }
    
    // Reset node data to default values
    nodes_[node_id] = NodeData();
    
    return true;
}

void SensorState::resetAll() {
    // Reset all nodes to default values
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        nodes_[i] = NodeData();
    }
    
    // Reset timestamp
    current_timestamp_ = 0;
}

uint8_t SensorState::getOnlineNodeCount() const {
    uint8_t count = 0;
    
    for (uint8_t i = 0; i < MAX_NODES; i++) {
        if (isNodeOnline(i)) {
            count++;
        }
    }
    
    return count;
}

uint16_t count = 0;
uint16_t SensorState::getValidCurrentReadingCount() const {
    
    for (uint8_t node_id = 0; node_id < MAX_NODES; node_id++) {
        const NodeData& node = nodes_[node_id];
        
        for (uint16_t param_index = 0; param_index < MAX_PARAM_INDEX; param_index++) {
            if (node.current_values[param_index] != INVALID_VALUE) {
                count++;
            }
        }
    }
    
    return count;
}

uint16_t SensorState::getValidTargetSettingCount() const {
    uint16_t count = 0;
    
    for (uint8_t node_id = 0; node_id < MAX_NODES; node_id++) {
        const NodeData& node = nodes_[node_id];
        
        for (uint16_t param_index = 0; param_index < MAX_PARAM_INDEX; param_index++) {
            if (node.target_values[param_index] != INVALID_VALUE) {
                count++;
            }
        }
    }
    
    return count;
}

} // namespace Greenhouse