#include <Arduino.h>
#include <WiFi.h>
#include "Config/secrets.h"
#include "Service/VoiceAssistant/voice_assistant_service.h"

// 实例化语音助手服务
VoiceAssistantService voiceAssistant;

void setupNetwork() {
    Serial.print("[Main] Connecting to Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[Main] Wi-Fi Connected!");
}

void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println("\n--- ESP32-S3 Smart Hub Booting ---");

    // 1. 连接基础网络
    setupNetwork();

    // 2. 初始化核心服务
    if (!voiceAssistant.init()) {
        Serial.println("[Main] Critical Error: Voice Assistant failed to init.");
        while (true) delay(1000); // 死机挂起
    }

    // 3. 启动后台 RTOS 任务
    voiceAssistant.start_task();

    Serial.println("--- Boot Complete. System Running! ---");
}

void loop() {
    // Main loop 变得极其清爽，只负责喂狗或极其轻量的状态监控。
    // 所有的繁重逻辑都在 VoiceTask (Core 1) 中并发运行。
    vTaskDelay(pdMS_TO_TICKS(1000));
}