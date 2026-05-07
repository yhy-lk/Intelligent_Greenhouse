/**
 * @file voice_assistant_bridge.cpp
 * @brief 语音助手服务的 C 语言桥接实现
 *
 * 使用 extern "C" 将 C++ 的 VoiceAssistantService 封装为
 * C 可调用的函数，供 GUI 层的 custom.c 调用。
 */
#include "voice_assistant_bridge.h"
#include "voice_assistant_service.h"
#include "esp_log.h"

static const char *TAG = "VA_BRIDGE";

// 全局唯一的语音助手服务实例（懒汉式单例）
static VoiceAssistantService *g_voiceAssistant = nullptr;

extern "C" {

bool voice_assistant_start(void) {
    // 如果实例已存在且任务已在运行，直接返回
    if (g_voiceAssistant != nullptr) {
        ESP_LOGW(TAG, "Voice Assistant instance already exists, starting task...");
        g_voiceAssistant->start_task();
        return true;
    }

    ESP_LOGI(TAG, "Creating Voice Assistant instance...");

    // 1. 创建实例
    g_voiceAssistant = new VoiceAssistantService();
    if (g_voiceAssistant == nullptr) {
        ESP_LOGE(TAG, "Failed to allocate VoiceAssistantService!");
        return false;
    }

    // 2. 初始化（I2S、PSRAM、百度 API）
    if (!g_voiceAssistant->init()) {
        ESP_LOGE(TAG, "Voice Assistant init failed!");
        delete g_voiceAssistant;
        g_voiceAssistant = nullptr;
        return false;
    }

    // 3. 启动后台任务
    g_voiceAssistant->start_task();

    ESP_LOGI(TAG, "Voice Assistant started successfully.");
    return true;
}

void voice_assistant_stop(void) {
    if (g_voiceAssistant == nullptr) {
        ESP_LOGW(TAG, "Voice Assistant is not running, nothing to stop.");
        return;
    }

    ESP_LOGI(TAG, "Stopping Voice Assistant...");

    // 1. 停止后台任务
    g_voiceAssistant->stop();

    // 2. 释放实例内存
    delete g_voiceAssistant;
    g_voiceAssistant = nullptr;

    ESP_LOGI(TAG, "Voice Assistant stopped and resources released.");
}

} // extern "C"