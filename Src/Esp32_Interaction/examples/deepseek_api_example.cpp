#include <Arduino.h>
#include <string>
#include <deepseek_api.h>
#include <os_service.h>

DeepSeekApi deepseek;

void setup() {

    Serial.begin();

    vTaskDelay(pdMS_TO_TICKS(5000)); // 等待串口稳定

    ESP_LOGI("DEEPSEEK", "Starting DeepSeekApi test...\n");

    std::string answer = deepseek.ask("Hello, DeepSeek!");

    ESP_LOGI("DEEPSEEK", "DeepSeek answer: %s\n", answer.c_str());
}

void loop() {
    // FreeRTOS模式下，loop函数为空，所有工作由任务处理
    // 这里只进行简单的延迟以避免占用CPU
    vTaskDelay(pdMS_TO_TICKS(1000 * 1000));
}

