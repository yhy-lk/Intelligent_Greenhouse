/**
 * @file can_network_service_example.cpp
 * @brief Example usage of CanNetworkService for ESP32-S3 greenhouse system
 * 
 * This example demonstrates how to use the CanNetworkService class to:
 * 1. Initialize CAN hardware with dependency injection
 * 2. Poll for received CAN messages and update SensorState
 * 3. Send control commands to greenhouse nodes
 * 4. Handle errors and display diagnostics
 * 
 * Hardware requirements:
 * - ESP32-S3 with TWAI/CAN interface
 * - CAN transceiver (e.g., SN65HVD230)
 * - GPIO 47 (TX) and GPIO 48 (RX) connected to transceiver
 * - Greenhouse nodes on CAN bus (for actual communication)
 */

#include <Arduino.h>
#include "../src/Service/CanNetworkService/can_network_service.h"
#include "../src/Service/SensorState/sensor_state.h"
#include "../src/Config/global_config.h"

// Create SensorState instance (digital twin)
Greenhouse::SensorState sensorState;

// Create CanNetworkService with dependency injection
Service::CanNetworkService canService(sensorState);

// CAN configuration using board-specific pins
Service::CanConfig canConfig(
    CAN_TX_PIN,     // TX pin from global_config.h
    CAN_RX_PIN,     // RX pin from global_config.h
    CAN_BAUD_RATE,  // Baud rate from global_config.h
    20,             // Queue size
    TWAI_MODE_NORMAL // Normal mode
);

// Function prototypes
void printSensorState(uint8_t nodeId);
void printDiagnostics();
void simulateNodeActivity();

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    delay(2000); // Wait for serial monitor
    
    Serial.println("\n========================================================");
    Serial.println("CanNetworkService Example - ESP32-S3 Greenhouse System");
    Serial.println("========================================================");
    
    // Step 1: Initialize CanNetworkService
    Serial.println("\n1. Initializing CanNetworkService...");
    if (canService.begin(canConfig)) {
        Serial.println("   ✓ CanNetworkService initialized successfully");
        Serial.printf("   Configuration: TX=GPIO%d, RX=GPIO%d, Baud=%lu bps\n",
            canConfig.tx_pin, canConfig.rx_pin, canConfig.baud_rate);
    } else {
        Serial.println("   ✗ Failed to initialize CanNetworkService!");
        Serial.println("   Please check hardware connections and power");
        while (1) { delay(1000); }
    }
    
    // Step 2: Initial diagnostics
    Serial.println("\n2. Initial diagnostics:");
    printDiagnostics();
    
    // Step 3: Demonstrate sensor state before any CAN messages
    Serial.println("\n3. Initial SensorState (digital twin):");
    Serial.println("   All nodes should be offline initially");
    for (uint8_t node = 0; node < 4; node++) {
        Serial.printf("   Node %d: %s\n", node, 
            sensorState.isNodeOnline(node) ? "Online" : "Offline");
    }
    
    Serial.println("\n4. Starting main loop...");
    Serial.println("   - Polling CAN messages every 100ms");
    Serial.println("   - Simulating node activity for demonstration");
    Serial.println("   - Press any key in serial monitor to send control command");
    Serial.println("========================================================");
}

void loop() {
    static uint32_t lastPollTime = 0;
    static uint32_t lastSimulationTime = 0;
    static uint32_t lastControlTime = 0;
    static uint8_t demoSequence = 0;
    
    uint32_t currentTime = millis();
    
    // Step 1: Poll for CAN messages (do this frequently)
    if (currentTime - lastPollTime >= 100) { // Poll every 100ms
        canService.poll();
        lastPollTime = currentTime;
    }
    
    // Step 2: Periodically simulate node activity (for demo purposes)
    if (currentTime - lastSimulationTime >= 5000) { // Every 5 seconds
        simulateNodeActivity();
        lastSimulationTime = currentTime;
    }
    
    // Step 3: Periodically print diagnostics
    if (currentTime - lastControlTime >= 10000) { // Every 10 seconds
        printDiagnostics();
        
        // Demonstrate sending control commands in a sequence
        switch (demoSequence % 4) {
            case 0:
                // Turn on water pump for node 1
                if (canService.sendControl(1, INDEX_WATER_PUMP, 1.0f)) {
                    Serial.println("\n[Demo] Sent: Turn ON water pump (Node 1)");
                }
                break;
                
            case 1:
                // Set temperature target for node 2
                if (canService.sendControl(2, INDEX_TEMPERATURE, 22.5f)) {
                    Serial.println("\n[Demo] Sent: Set temperature to 22.5°C (Node 2)");
                }
                break;
                
            case 2:
                // Turn off water pump for node 1
                if (canService.sendControl(1, INDEX_WATER_PUMP, 0.0f)) {
                    Serial.println("\n[Demo] Sent: Turn OFF water pump (Node 1)");
                }
                break;
                
            case 3:
                // Set light intensity for node 3
                if (canService.sendControl(3, INDEX_LIGHT_INTENSITY, 75.0f)) {
                    Serial.println("\n[Demo] Sent: Set light intensity to 75 Lux (Node 3)");
                }
                break;
        }
        
        demoSequence++;
        lastControlTime = currentTime;
    }
    
    // Step 4: Check for serial input to send manual commands
    if (Serial.available() > 0) {
        char input = Serial.read();
        Serial.println("\n[Manual Control] Press:");
        Serial.println("  1 - Turn ON water pump (Node 1)");
        Serial.println("  2 - Turn OFF water pump (Node 1)");
        Serial.println("  3 - Set temperature 22.5°C (Node 2)");
        Serial.println("  4 - Set light intensity 75 Lux (Node 3)");
        Serial.println("  5 - Print sensor state");
        Serial.println("  6 - Print diagnostics");
        Serial.println("  7 - Reset statistics");
        
        switch (input) {
            case '1':
                canService.sendControl(1, INDEX_WATER_PUMP, 1.0f);
                Serial.println("Sent: Turn ON water pump (Node 1)");
                break;
            case '2':
                canService.sendControl(1, INDEX_WATER_PUMP, 0.0f);
                Serial.println("Sent: Turn OFF water pump (Node 1)");
                break;
            case '3':
                canService.sendControl(2, INDEX_TEMPERATURE, 22.5f);
                Serial.println("Sent: Set temperature to 22.5°C (Node 2)");
                break;
            case '4':
                canService.sendControl(3, INDEX_LIGHT_INTENSITY, 75.0f);
                Serial.println("Sent: Set light intensity to 75 Lux (Node 3)");
                break;
            case '5':
                Serial.println("\nCurrent SensorState:");
                for (uint8_t node = 0; node < 4; node++) {
                    printSensorState(node);
                }
                break;
            case '6':
                printDiagnostics();
                break;
            case '7':
                canService.resetStatistics();
                Serial.println("Statistics reset");
                break;
        }
        
        // Clear any remaining input
        while (Serial.available() > 0) Serial.read();
    }
    
    delay(10); // Small delay to prevent watchdog issues
}

/**
 * @brief Print detailed sensor state for a node
 * @param nodeId Node ID to display (0-15)
 */
void printSensorState(uint8_t nodeId) {
    if (nodeId >= 16) return; // Validate node ID
    
    Serial.printf("\nNode %d: ", nodeId);
    if (sensorState.isNodeOnline(nodeId)) {
        Serial.printf("Online (last seen: %lu ms ago)\n", 
            millis() - sensorState.getNodeLastSeen(nodeId));
        
        // Check for key parameters
        float temp = sensorState.getCurrentValue(nodeId, INDEX_TEMPERATURE);
        float humidity = sensorState.getCurrentValue(nodeId, INDEX_HUMIDITY_AIR);
        float soilHumidity = sensorState.getCurrentValue(nodeId, INDEX_HUMIDITY_SOIL);
        float light = sensorState.getCurrentValue(nodeId, INDEX_LIGHT_INTENSITY);
        
        if (sensorState.hasCurrentValue(nodeId, INDEX_TEMPERATURE)) {
            Serial.printf("   Temperature: %.1f°C\n", temp);
        }
        if (sensorState.hasCurrentValue(nodeId, INDEX_HUMIDITY_AIR)) {
            Serial.printf("   Air Humidity: %.1f%%\n", humidity);
        }
        if (sensorState.hasCurrentValue(nodeId, INDEX_HUMIDITY_SOIL)) {
            Serial.printf("   Soil Humidity: %.1f%%\n", soilHumidity);
        }
        if (sensorState.hasCurrentValue(nodeId, INDEX_LIGHT_INTENSITY)) {
            Serial.printf("   Light: %.0f Lux\n", light);
        }
        
        // Check for any target settings
        bool hasTargets = false;
        for (uint8_t param = 0; param < 0x60; param++) {
            if (sensorState.hasTargetValue(nodeId, param)) {
                if (!hasTargets) {
                    Serial.println("   Target Settings:");
                    hasTargets = true;
                }
                float target = sensorState.getTargetValue(nodeId, param);
                Serial.printf("     Param 0x%02X: %.2f\n", param, target);
            }
        }
        if (!hasTargets) {
            Serial.println("   No target settings");
        }
    } else {
        Serial.println("Offline");
    }
}

/**
 * @brief Print comprehensive diagnostics
 */
void printDiagnostics() {
    Serial.println("\n=== CanNetworkService Diagnostics ===");
    
    // Service status
    Serial.printf("Service Status: %s\n", 
        canService.isInitialized() ? "Running" : "Stopped");
    
    // Message statistics
    Serial.printf("Message Statistics:\n");
    Serial.printf("  Received: %lu\n", canService.getRxCount());
    Serial.printf("  Sent: %lu\n", canService.getTxCount());
    Serial.printf("  Parse Errors: %lu\n", canService.getParseErrorCount());
    Serial.printf("  Bus Errors: %lu\n", canService.getBusErrorCount());
    
    // Online node count
    uint8_t onlineNodes = 0;
    for (uint8_t node = 0; node < 16; node++) {
        if (sensorState.isNodeOnline(node)) onlineNodes++;
    }
    Serial.printf("Online Nodes: %d/16\n", onlineNodes);
    
    // TWAI status
    twai_status_info_t status = canService.getStatus();
    const char* stateStr = 
        (status.state == TWAI_STATE_RUNNING) ? "Running" :
        (status.state == TWAI_STATE_STOPPED) ? "Stopped" :
        (status.state == TWAI_STATE_BUS_OFF) ? "Bus Off" :
        (status.state == TWAI_STATE_RECOVERING) ? "Recovering" : "Unknown";
    
    Serial.printf("TWAI Status: %s\n", stateStr);
    Serial.printf("Error Counters: TX=%d, RX=%d, Bus=%d\n",
        status.tx_error_counter, status.rx_error_counter, status.bus_error_count);
    
    Serial.println("=====================================");
}

/**
 * @brief Simulate node activity for demonstration purposes
 * 
 * In a real system, this would be actual CAN messages from nodes.
 * For demonstration, we simulate nodes coming online and sending data.
 */
void simulateNodeActivity() {
    static uint8_t simCycle = 0;
    
    switch (simCycle % 4) {
        case 0:
            // Simulate node 1 reporting temperature
            Serial.println("\n[Simulation] Node 1 reports: Temperature = 23.5°C");
            sensorState.updateState(1, INDEX_TEMPERATURE, 23.5f, false);
            sensorState.markNodeOnline(1);
            break;
            
        case 1:
            // Simulate node 2 reporting humidity
            Serial.println("\n[Simulation] Node 2 reports: Air Humidity = 65.2%");
            sensorState.updateState(2, INDEX_HUMIDITY_AIR, 65.2f, false);
            sensorState.markNodeOnline(2);
            break;
            
        case 2:
            // Simulate node 3 reporting light intensity
            Serial.println("\n[Simulation] Node 3 reports: Light Intensity = 850 Lux");
            sensorState.updateState(3, INDEX_LIGHT_INTENSITY, 850.0f, false);
            sensorState.markNodeOnline(3);
            break;
            
        case 3:
            // Simulate node 1 acknowledging target setting
            Serial.println("\n[Simulation] Node 1 acknowledges: Water Pump = ON");
            sensorState.updateState(1, INDEX_WATER_PUMP, 1.0f, true);
            sensorState.markNodeOnline(1);
            break;
    }
    
    simCycle++;
}

/**
 * @brief Handle critical errors (not implemented in this example)
 * 
 * In a production system, you would implement proper error handling,
 * such as resetting the CAN controller or entering a safe state.
 */
void handleCriticalError() {
    Serial.println("\n[ERROR] Critical CAN bus error detected!");
    Serial.println("Attempting recovery...");
    
    // Stop and restart the service
    canService.stop();
    delay(1000);
    
    if (canService.begin(canConfig)) {
        Serial.println("Recovery successful!");
    } else {
        Serial.println("Recovery failed! System may need manual intervention.");
    }
}