/**
 * @file test_greenhouse_protocol.cpp
 * @brief Unit tests for Greenhouse CAN Bus Protocol (PlatformIO Unity Version)
 */

#include <Arduino.h>
#include "../src/Service/Protocol/CAN/can_protocol.h"
#include <string.h>
#include <math.h>
#include <unity.h>

// ============================================================================
// Unity 必须的两个钩子函数 (即使为空也必须声明)
// ============================================================================
void setUp(void) {
    // 每次执行 RUN_TEST 前都会运行这里，可以用来初始化变量
}

void tearDown(void) {
    // 每次执行 RUN_TEST 后都会运行这里，可以用来清理内存
}

// ============================================================================
// Test Functions (注意：返回值必须是 void，参数必须是 void)
// ============================================================================

void test_id_build_extract(void) {
    // Test basic ID building and extraction
    for (uint8_t func = 0; func <= MAX_FUNC_CODE; func++) {
        for (uint8_t node = 0; node <= MAX_NODE_ID; node += 16) { // Skip some to speed up
            uint16_t id = greenhouse_build_id(func, node);
            
            uint8_t extracted_func = greenhouse_get_func_from_id(id);
            uint8_t extracted_node = greenhouse_get_node_from_id(id);
            
            // 使用 Unity 的断言宏
            TEST_ASSERT_EQUAL_UINT8(func, extracted_func);
            TEST_ASSERT_EQUAL_UINT8(node, extracted_node);
            
            // Verify bit masking works
            uint16_t id2 = greenhouse_build_id(func | 0xF0, node | 0x80);
            TEST_ASSERT_EQUAL_UINT8(func, greenhouse_get_func_from_id(id2));
            TEST_ASSERT_EQUAL_UINT8(node, greenhouse_get_node_from_id(id2));
        }
    }
    
    // Test edge cases
    uint16_t id = greenhouse_build_id(0xF, 0x7F); // Max values
    TEST_ASSERT_EQUAL_UINT8(0xF, greenhouse_get_func_from_id(id));
    TEST_ASSERT_EQUAL_UINT8(0x7F, greenhouse_get_node_from_id(id));
}

void test_value_conversion(void) {
    // Test temperature scaling (×100)
    float temp = 25.5f;
    uint32_t raw = greenhouse_float_to_u32(INDEX_TEMPERATURE, temp);
    TEST_ASSERT_EQUAL_UINT32(2550, raw); // 25.5 × 100 = 2550
    
    float recovered = greenhouse_u32_to_float(INDEX_TEMPERATURE, raw);
    // 使用 Unity 专用的浮点数断言 (比较两个浮点数差值小于指定阈值)
    TEST_ASSERT_FLOAT_WITHIN(0.01f, temp, recovered);
    
    // Test humidity scaling (×100)
    float humidity = 65.25f;
    raw = greenhouse_float_to_u32(INDEX_HUMIDITY_AIR, humidity);
    TEST_ASSERT_EQUAL_UINT32(6525, raw); // 65.25 × 100 = 6525
    
    recovered = greenhouse_u32_to_float(INDEX_HUMIDITY_AIR, raw);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, humidity, recovered);
    
    // Test parameters without scaling
    float co2 = 415.0f;
    raw = greenhouse_float_to_u32(INDEX_CO2_LEVEL, co2);
    TEST_ASSERT_EQUAL_UINT32(415, raw); // No scaling
    
    recovered = greenhouse_u32_to_float(INDEX_CO2_LEVEL, raw);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, co2, recovered);
    
    // Test rounding
    float temp2 = 25.49f;
    raw = greenhouse_float_to_u32(INDEX_TEMPERATURE, temp2);
    TEST_ASSERT_EQUAL_UINT32(2549, raw); // Should round down
    
    float temp3 = 25.51f;
    raw = greenhouse_float_to_u32(INDEX_TEMPERATURE, temp3);
    TEST_ASSERT_EQUAL_UINT32(2551, raw); // Should round up
}

void test_packet_building(void) {
    uint8_t buffer[CAN_DATA_SIZE];
    
    // Test building a temperature packet
    float temp = 23.75f;
    greenhouse_build_packet(buffer, INDEX_TEMPERATURE, temp);
    
    // Verify buffer structure
    TEST_ASSERT_EQUAL_UINT8(INDEX_TEMPERATURE, buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(0, buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buffer[2]);
    TEST_ASSERT_EQUAL_UINT8(0, buffer[3]);
    
    // Extract value (little-endian)
    uint32_t raw_value = *(uint32_t*)&buffer[4];
    TEST_ASSERT_EQUAL_UINT32(2375, raw_value); // 23.75 × 100 = 2375
    
    // Test building with raw value
    uint8_t buffer2[CAN_DATA_SIZE];
    greenhouse_build_packet_raw(buffer2, INDEX_LIGHT_INTENSITY, 12345);
    
    TEST_ASSERT_EQUAL_UINT8(INDEX_LIGHT_INTENSITY, buffer2[0]);
    TEST_ASSERT_EQUAL_UINT8(0, buffer2[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buffer2[2]);
    TEST_ASSERT_EQUAL_UINT8(0, buffer2[3]);
    
    uint32_t raw_value2 = *(uint32_t*)&buffer2[4];
    TEST_ASSERT_EQUAL_UINT32(12345, raw_value2);
}

void test_packet_parsing(void) {
    // Build a test packet
    uint8_t buffer[CAN_DATA_SIZE];
    float temp = 28.5f;
    greenhouse_build_packet(buffer, INDEX_TEMPERATURE, temp);
    
    // Build a CAN ID
    uint16_t can_id = greenhouse_build_id(FUNC_REPORT, 0x2A); // Node 42
    
    // Parse the packet
    ProtocolResult result;
    bool success = greenhouse_parse_packet(can_id, buffer, &result);
    
    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_TRUE(result.is_valid);
    TEST_ASSERT_EQUAL_UINT8(FUNC_REPORT, result.func_code);
    TEST_ASSERT_EQUAL_UINT8(0x2A, result.node_id);
    TEST_ASSERT_EQUAL_UINT8(INDEX_TEMPERATURE, result.param_index);
    TEST_ASSERT_EQUAL_UINT32(2850, result.raw_value); // 28.5 × 100
    TEST_ASSERT_TRUE(result.is_scaled);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, temp, result.scaled_value);
    
    // Test invalid packet (non-zero reserved bytes)
    uint8_t invalid_buffer[CAN_DATA_SIZE];
    memcpy(invalid_buffer, buffer, CAN_DATA_SIZE);
    invalid_buffer[1] = 0x01; // Make reserved byte non-zero
    
    ProtocolResult result2;
    success = greenhouse_parse_packet(can_id, invalid_buffer, &result2);
    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_FALSE(result2.is_valid);
    
    // Test invalid parameter index
    uint8_t invalid_index_buffer[CAN_DATA_SIZE];
    memset(invalid_index_buffer, 0, CAN_DATA_SIZE);
    invalid_index_buffer[0] = 0xFF; // Invalid index
    
    ProtocolResult result3;
    success = greenhouse_parse_packet(can_id, invalid_index_buffer, &result3);
    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_FALSE(result3.is_valid);
}

void test_frame_validation(void) {
    // Create a valid frame
    CanDataFrame valid_frame;
    valid_frame.index = INDEX_WATER_PUMP;
    valid_frame.reserved[0] = 0;
    valid_frame.reserved[1] = 0;
    valid_frame.reserved[2] = 0;
    valid_frame.value = 1; // ON
    
    TEST_ASSERT_TRUE(greenhouse_validate_frame(&valid_frame));
    
    // Test invalid frame (non-zero reserved)
    CanDataFrame invalid_frame1;
    invalid_frame1.index = INDEX_WATER_PUMP;
    invalid_frame1.reserved[0] = 0;
    invalid_frame1.reserved[1] = 1; // Non-zero
    invalid_frame1.reserved[2] = 0;
    invalid_frame1.value = 1;
    
    TEST_ASSERT_FALSE(greenhouse_validate_frame(&invalid_frame1));
    
    // Test invalid frame (invalid index)
    CanDataFrame invalid_frame2;
    invalid_frame2.index = 0x99; // Invalid index
    invalid_frame2.reserved[0] = 0;
    invalid_frame2.reserved[1] = 0;
    invalid_frame2.reserved[2] = 0;
    invalid_frame2.value = 1234;
    
    TEST_ASSERT_FALSE(greenhouse_validate_frame(&invalid_frame2));
}

void test_utility_functions(void) {
    // 使用 Unity 的字符串比较断言，报错时会显示差异
    TEST_ASSERT_EQUAL_STRING("Temperature", greenhouse_get_param_name(INDEX_TEMPERATURE));
    TEST_ASSERT_EQUAL_STRING("Water Pump", greenhouse_get_param_name(INDEX_WATER_PUMP));
    TEST_ASSERT_EQUAL_STRING("Unknown Parameter", greenhouse_get_param_name(INDEX_INVALID));
    
    TEST_ASSERT_EQUAL_STRING("Alert", greenhouse_get_func_name(FUNC_ALERT));
    TEST_ASSERT_EQUAL_STRING("Report", greenhouse_get_func_name(FUNC_REPORT));
    TEST_ASSERT_EQUAL_STRING("Unknown Function", greenhouse_get_func_name(0xFF));
    
    TEST_ASSERT_EQUAL_FLOAT(100.0f, greenhouse_get_scaling_factor(INDEX_TEMPERATURE));
    TEST_ASSERT_EQUAL_FLOAT(100.0f, greenhouse_get_scaling_factor(INDEX_HUMIDITY_AIR));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, greenhouse_get_scaling_factor(INDEX_CO2_LEVEL));
    
    TEST_ASSERT_TRUE(greenhouse_needs_scaling(INDEX_TEMPERATURE));
    TEST_ASSERT_TRUE(greenhouse_needs_scaling(INDEX_SOIL_PH));
    TEST_ASSERT_FALSE(greenhouse_needs_scaling(INDEX_CO2_LEVEL));
    TEST_ASSERT_FALSE(greenhouse_needs_scaling(INDEX_WATER_PUMP));
}

// ============================================================================
// Arduino Entry Point
// ============================================================================

void setup() {
    // 等待 2 秒，让串口助手有时间连上，否则可能看不到前面的打印
    delay(2000); 

    UNITY_BEGIN(); // 初始化 Unity 测试框架
    
    RUN_TEST(test_id_build_extract);
    RUN_TEST(test_value_conversion);
    RUN_TEST(test_packet_building);
    RUN_TEST(test_packet_parsing);
    RUN_TEST(test_frame_validation);
    RUN_TEST(test_utility_functions);
    
    UNITY_END(); // 打印测试总结报告
}

void loop() {
    // 测试跑完后在这里死循环挂起
    delay(1000);
}