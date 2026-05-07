#pragma once
#include <Arduino.h>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "inmp441_driver.h"
#include "max98357a_driver.h"
#include "baidu_api.h"
#include "deepseek_api.h" // 🚀 引入 DeepSeek 大脑

#include "app_config.h" // 统一使用全局配置文件

enum class VoiceState {
    WAITING,
    RECORDING,
    PROCESSING_STT,
    PROCESSING_LLM,
    PROCESSING_TTS,
    PLAYBACK
};

class VoiceAssistantService {
public:
    VoiceAssistantService();
    ~VoiceAssistantService();

    bool init();
    void start_task(UBaseType_t priority = TASK_PRIORITY_NORMAL, uint32_t stack_size = TASK_STACK_SIZE_MEDIUM);

private:
    static void task_handler(void* pvParameters);
    void state_machine_tick();

    Inmp441Driver mic;
    Max98357aDriver amp;
    BaiduApi baidu_api;
    DeepSeekApi deepseek_api; // 🚀 实例化大脑

    VoiceState current_state;
    
    int16_t* record_buffer;
    int recorded_samples_count;
    int downloaded_bytes_count;
    std::string current_text;

    static const int SAMPLE_RATE = 16000;
    static const int RECORD_SECONDS_MAX = 10;
    static const int MAX_PSRAM_SAMPLES = SAMPLE_RATE * RECORD_SECONDS_MAX;
    static const int MAX_PSRAM_BYTES = MAX_PSRAM_SAMPLES * sizeof(int16_t);
    static const int FRAME_SIZE = 256;
    
    static const int VOICE_THRESHOLD = 500;
    static const int SILENCE_THRESHOLD = 300;
    static const int SILENCE_TIMEOUT_MS = 1500;
    int current_silent_samples;
};