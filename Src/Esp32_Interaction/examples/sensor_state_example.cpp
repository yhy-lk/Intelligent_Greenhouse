/**
 * @file sensor_state_example.cpp
 * @brief Example usage of SensorState Digital Twin Service Layer
 * 
 * This example demonstrates how to use the SensorState class to maintain
 * digital twin state for a smart greenhouse system.
 */

#include <Arduino.h>
#include "../src/Service/Protocol/SensorState/sensor_state.h"

using namespace Greenhouse;

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for serial monitor
    
    Serial.println("\n========================================");
    Serial.println("SensorState Digital Twin Example");
    Serial.println("========================================");
    
    // ========================================================================
    // Example 1: Basic SensorState Creation and Configuration
    // ========================================================================
    Serial.println("\n1. Basic SensorState Creation:");
    
    // Create SensorState instance with 1 minute timeout (for demo purposes)
    SensorState sensorState(60 * 1000); // 1 minute timeout
    
    Serial.printf("SensorState created with %d ms timeout\n", sensorState.getNodeTimeout());
    Serial.printf("Maximum nodes: %d\n", MAX_NODES);
    Serial.printf("Maximum parameter indices: %d\n", MAX_PARAM_INDEX);
    
    // ========================================================================
    // Example 2: Ingesting Data from CAN Protocol Layer
    // ========================================================================
    Serial.println("\n2. Ingesting Data from CAN Protocol Layer:");
    
    // Simulate receiving temperature reading from node 5 (25.5°C)
    sensorState.updateTimestamp(1000); // Set current timestamp to 1000ms
    bool success = sensorState.updateState(5, INDEX_TEMPERATURE, 25.5f, false);
    Serial.printf("Update temperature reading from node 5: %s\n", success ? "SUCCESS" : "FAILED");
    
    // Simulate receiving target temperature setting acknowledgment from node 5 (26.0°C)
    success = sensorState.updateState(5, INDEX_TEMPERATURE, 26.0f, true);
    Serial.printf("Update target temperature for node 5: %s\n", success ? "SUCCESS" : "FAILED");
    
    // Simulate receiving air humidity reading from node 5 (65.2%)
    success = sensorState.updateState(5, INDEX_HUMIDITY_AIR, 65.2f, false);
    Serial.printf("Update air humidity reading from node 5: %s\n", success ? "SUCCESS" : "FAILED");
    
    // ========================================================================
    // Example 3: Checking Node Status
    // ========================================================================
    Serial.println("\n3. Checking Node Status:");
    
    bool isOnline = sensorState.isNodeOnline(5);
    uint32_t lastSeen = sensorState.getNodeLastSeen(5);
    Serial.printf("Node 5 is %s (last seen at %lu ms)\n", 
                  isOnline ? "ONLINE" : "OFFLINE", lastSeen);
    
    // Node 3 hasn't communicated yet
    isOnline = sensorState.isNodeOnline(3);
    Serial.printf("Node 3 is %s (expected: OFFLINE)\n", isOnline ? "ONLINE" : "OFFLINE");
    
    // ========================================================================
    // Example 4: Retrieving Data for JSON/AI Logic Layer
    // ========================================================================
    Serial.println("\n4. Retrieving Data for Upper Layers:");
    
    float currentTemp = sensorState.getCurrentValue(5, INDEX_TEMPERATURE);
    float targetTemp = sensorState.getTargetValue(5, INDEX_TEMPERATURE);
    Serial.printf("Node 5 Temperature: Current=%.1f°C, Target=%.1f°C\n", currentTemp, targetTemp);
    
    float currentHumidity = sensorState.getCurrentValue(5, INDEX_HUMIDITY_AIR);
    Serial.printf("Node 5 Humidity: Current=%.1f%%\n", currentHumidity);
    
    // Try to get non-existent data
    float nonExistent = sensorState.getCurrentValue(5, INDEX_WATER_PUMP);
    if (nonExistent == INVALID_VALUE) {
        Serial.println("Water pump status: Not available (expected INVALID_VALUE)");
    }
    
    // ========================================================================
    // Example 5: Error Detection
    // ========================================================================
    Serial.println("\n5. Error Detection:");
    
    bool hasError = sensorState.hasError(5);
    Serial.printf("Node 5 has error: %s\n", hasError ? "YES" : "NO");
    
    // Check deviation for temperature (0.5°C difference, within 2°C tolerance)
    bool excessiveDeviation = sensorState.hasExcessiveDeviation(5, INDEX_TEMPERATURE, 2.0f);
    Serial.printf("Temperature deviation excessive (>2°C): %s\n", 
                  excessiveDeviation ? "YES" : "NO");
    
    // ========================================================================
    // Example 6: Advanced Usage - Direct Node Data Access
    // ========================================================================
    Serial.println("\n6. Advanced Usage - Direct Node Data Access:");
    
    const NodeData* nodeData = sensorState.getNodeData(5);
    if (nodeData) {
        Serial.printf("Node 5 data pointer: 0x%p\n", nodeData);
        Serial.printf("Node 5 has %d valid current readings\n", 
                     sensorState.getValidCurrentReadingCount());
        Serial.printf("Node 5 has %d valid target settings\n",
                     sensorState.getValidTargetSettingCount());
    }
    
    // ========================================================================
    // Example 7: System-wide Statistics
    // ========================================================================
    Serial.println("\n7. System-wide Statistics:");
    
    // Update more nodes to show statistics
    sensorState.updateTimestamp(2000);
    sensorState.updateState(2, INDEX_TEMPERATURE, 24.0f, false);
    sensorState.updateState(2, INDEX_TEMPERATURE, 25.0f, true);
    sensorState.markNodeOnline(2);
    
    sensorState.updateState(8, INDEX_LIGHT_INTENSITY, 850.0f, false);
    sensorState.updateState(8, INDEX_LIGHT_INTENSITY, 900.0f, true);
    sensorState.markNodeOnline(8);
    
    uint8_t onlineCount = sensorState.getOnlineNodeCount();
    uint16_t totalCurrentReadings = sensorState.getValidCurrentReadingCount();
    uint16_t totalTargetSettings = sensorState.getValidTargetSettingCount();
    
    Serial.printf("Online nodes: %d/%d\n", onlineCount, MAX_NODES);
    Serial.printf("Total current readings: %d\n", totalCurrentReadings);
    Serial.printf("Total target settings: %d\n", totalTargetSettings);
    
    // ========================================================================
    // Example 8: Reset Operations
    // ========================================================================
    Serial.println("\n8. Reset Operations:");
    
    // Reset a single node
    success = sensorState.resetNode(5);
    Serial.printf("Reset node 5: %s\n", success ? "SUCCESS" : "FAILED");
    
    // Verify node 5 data is now invalid
    currentTemp = sensorState.getCurrentValue(5, INDEX_TEMPERATURE);
    if (currentTemp == INVALID_VALUE) {
        Serial.println("Node 5 temperature is now INVALID_VALUE (reset successful)");
    }
    
    // Reset all nodes
    sensorState.resetAll();
    Serial.println("All nodes reset to factory defaults");
    
    onlineCount = sensorState.getOnlineNodeCount();
    Serial.printf("Online nodes after reset: %d (expected: 0)\n", onlineCount);
    
    // ========================================================================
    // Example 9: Invalid Input Handling
    // ========================================================================
    Serial.println("\n9. Invalid Input Handling:");
    
    // Try to update with invalid node ID
    success = sensorState.updateState(20, INDEX_TEMPERATURE, 25.0f, false);
    Serial.printf("Update with invalid node ID (20): %s (expected: FAILED)\n", 
                  success ? "SUCCESS" : "FAILED");
    
    // Try to update with invalid parameter index
    success = sensorState.updateState(5, 0xFF, 25.0f, false);
    Serial.printf("Update with invalid parameter index (0xFF): %s (expected: FAILED)\n",
                  success ? "SUCCESS" : "FAILED");
    
    // Try to get data from invalid node
    float value = sensorState.getCurrentValue(20, INDEX_TEMPERATURE);
    if (value == INVALID_VALUE) {
        Serial.println("Get from invalid node returns INVALID_VALUE (expected)");
    }
    
    Serial.println("\n========================================");
    Serial.println("Example Complete");
    Serial.println("========================================");
}

void loop() {
    // Simple heartbeat to show the example is running
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 5000) {
        Serial.println("SensorState example is running...");
        lastPrint = millis();
    }
    
    delay(100);
}