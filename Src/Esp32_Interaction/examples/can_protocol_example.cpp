/**
 * @file greenhouse_protocol_example.cpp
 * @brief Example usage of Greenhouse CAN Bus Protocol
 * 
 * This example demonstrates how to use the greenhouse protocol library
 * for packing, unpacking, and processing CAN messages.
 */

#include <Arduino.h>
#include "../src/Service/Protocol/CAN/can_protocol.h"

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for serial monitor
    
    Serial.println("\n========================================");
    Serial.println("Greenhouse Protocol Example");
    Serial.println("========================================");
    
    // Example 1: Build and parse a temperature reading
    Serial.println("\n1. Temperature Reading Example:");
    
    // Build a CAN ID for node 10 sending a report
    uint16_t can_id = GreenhouseProtocol::buildID(FUNC_REPORT, 0b0001010);
    Serial.printf("CAN ID: 0x%03X\n", can_id);
    Serial.printf("  Function: %s (0x%X)\n", 
                  GreenhouseProtocol::getFuncName(GreenhouseProtocol::getFuncFromID(can_id)),
                  GreenhouseProtocol::getFuncFromID(can_id));
    Serial.printf("  Node: %d\n", GreenhouseProtocol::getNodeFromID(can_id));
    
    // Create temperature data (25.5°C)
    float temperature = 25.5f;
    uint8_t can_data[GreenhouseProtocol::getDataFrameSize()];
    GreenhouseProtocol::buildPacket(can_data, INDEX_TEMPERATURE, temperature);
    
    Serial.printf("Temperature: %.2f°C\n", temperature);
    Serial.print("CAN Data: ");
    for (size_t i = 0; i < sizeof(can_data); i++) {
        Serial.printf("%02X ", can_data[i]);
    }
    Serial.println();
    
    // Parse the data back
    ProtocolResult result = GreenhouseProtocol::createResult();
    if (GreenhouseProtocol::parsePacket(can_id, can_data, result)) {
        Serial.printf("Parsed: %s = %.2f°C\n", 
                     GreenhouseProtocol::getParamName(result.param_index),
                     result.scaled_value);
    }
    
    // Example 2: Control a water pump
    Serial.println("\n2. Water Pump Control Example:");
    
    can_id = GreenhouseProtocol::buildID(FUNC_WRITE_SET, 1); // Master node 1
    bool pump_on = true;
    GreenhouseProtocol::buildPacketRaw(can_data, INDEX_WATER_PUMP, pump_on ? 1 : 0);
    
    Serial.printf("CAN ID: 0x%03X\n", can_id);
    Serial.printf("Water Pump: %s\n", pump_on ? "ON" : "OFF");
    Serial.print("CAN Data: ");
    for (size_t i = 0; i < sizeof(can_data); i++) {
        Serial.printf("%02X ", can_data[i]);
    }
    Serial.println();
    
    // Example 3: Validate frames
    Serial.println("\n3. Frame Validation Example:");
    
    // Valid frame
    CanDataFrame valid_frame;
    valid_frame.index = INDEX_LIGHT_INTENSITY;
    valid_frame.reserved[0] = 0;
    valid_frame.reserved[1] = 0;
    valid_frame.reserved[2] = 0;
    valid_frame.value = 850; // 850 Lux
    
    bool is_valid = GreenhouseProtocol::validateFrame(&valid_frame);
    Serial.printf("Valid frame: %s\n", is_valid ? "YES" : "NO");
    
    // Invalid frame (non-zero reserved bytes)
    CanDataFrame invalid_frame;
    invalid_frame.index = INDEX_LIGHT_INTENSITY;
    invalid_frame.reserved[0] = 0;
    invalid_frame.reserved[1] = 1; // Non-zero
    invalid_frame.reserved[2] = 0;
    invalid_frame.value = 850;
    
    is_valid = GreenhouseProtocol::validateFrame(&invalid_frame);
    Serial.printf("Invalid frame: %s\n", is_valid ? "YES" : "NO");
    
    // Example 4: Scaling demonstration
    Serial.println("\n4. Scaling Demonstration:");
    
    float test_values[] = {23.75f, 65.25f, 415.0f, 7.2f};
    ParameterIndex test_indices[] = {
        INDEX_TEMPERATURE,
        INDEX_HUMIDITY_AIR,
        INDEX_CO2_LEVEL,
        INDEX_SOIL_PH
    };
    
    for (int i = 0; i < 4; i++) {
        ParameterIndex idx = test_indices[i];
        float value = test_values[i];
        
        uint32_t raw = GreenhouseProtocol::floatToU32(idx, value);
        float recovered = GreenhouseProtocol::u32ToFloat(idx, raw);
        
        Serial.printf("%s: %.2f -> %lu -> %.2f (Scaling: %s)\n",
                     GreenhouseProtocol::getParamName(idx),
                     value, raw, recovered,
                     GreenhouseProtocol::needsScaling(idx) ? "×100" : "1:1");
    }
    
    Serial.println("\n========================================");
    Serial.println("Example Complete");
    Serial.println("========================================");
}

void loop() {
    // Nothing to do in loop for this example
    delay(1000);
}