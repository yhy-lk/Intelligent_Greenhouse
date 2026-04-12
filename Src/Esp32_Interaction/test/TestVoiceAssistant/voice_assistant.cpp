#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"
#include "voice_assistant_service.h"
#include "esp_log.h" // 引入ESP-IDF日志宏

// 定义当前文件的日志标签
static const char *TAG = "MAIN";

// 实例化语音助手服务
VoiceAssistantService voiceAssistant;

void setupNetwork() {
    ESP_LOGI(TAG, "Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        vTaskDelay(pdMS_TO_TICKS(500));
        ESP_LOGD(TAG, "Waiting for Wi-Fi connection..."); // 避免使用点号刷屏
    }
    ESP_LOGI(TAG, "Wi-Fi Connected!");
}

void setup() {
    vTaskDelay(pdMS_TO_TICKS(3000));
    ESP_LOGI(TAG, "--- ESP32-S3 Smart Hub Booting ---");

    // 1. 连接基础网络
    setupNetwork();

    // 2. 初始化核心服务
    if (!voiceAssistant.init()) {
        ESP_LOGE(TAG, "Critical Error: Voice Assistant failed to init.");
        while (true) vTaskDelay(pdMS_TO_TICKS(1000)); // 死机挂起，替换为 vTaskDelay
    }

    // 3. 启动后台 RTOS 任务
    voiceAssistant.start_task();

    ESP_LOGI(TAG, "--- Boot Complete. System Running! ---");
}

void loop() {
    // Main loop 变得极其清爽，只负责喂狗或极其轻量的状态监控。
    // 所有的繁重逻辑都在 VoiceTask (Core 1) 中并发运行。
    vTaskDelay(pdMS_TO_TICKS(1000));
}