#include "voice_assistant_service.h"
#include "i2s_hal.h"
#include "secrets.h"
#include <driver/i2s.h> // 引入底层驱动，用于动态切换采样率
#include "esp_log.h"    // 引入ESP-IDF日志宏

// 定义当前文件的日志标签
static const char *TAG = "VOICE_SERVICE";

VoiceAssistantService::VoiceAssistantService() 
    : current_state(VoiceState::WAITING), 
      record_buffer(nullptr), 
      recorded_samples_count(0), 
      downloaded_bytes_count(0),
      current_silent_samples(0) {}

VoiceAssistantService::~VoiceAssistantService() {
    if (record_buffer != nullptr) {
        free(record_buffer);
    }
}

bool VoiceAssistantService::init() {
    ESP_LOGI(TAG, "Initializing Audio Engine...");
    if (!I2SHal::getInstance().begin()) {
        ESP_LOGE(TAG, "I2S Engine Failed!");
        return false;
    }

    if (psramFound()) {
        record_buffer = (int16_t*)ps_malloc(MAX_PSRAM_BYTES);
        ESP_LOGI(TAG, "Allocated %d bytes in PSRAM.", MAX_PSRAM_BYTES);
    } else {
        ESP_LOGE(TAG, "PSRAM not found! Allocation failed.");
        return false;
    }

    if (!baidu_api.init(std::string(BAIDU_API_KEY), std::string(BAIDU_SECRET_KEY))) {
        ESP_LOGE(TAG, "Baidu API Init Failed!");
        return false;
    }

    ESP_LOGI(TAG, "Initialization Complete.");
    return true;
}

void VoiceAssistantService::start_task(UBaseType_t priority, uint32_t stack_size) {
    xTaskCreatePinnedToCore(
        task_handler,
        "VoiceTask",
        stack_size,
        this,
        priority,
        NULL,
        1 
    );
}

void VoiceAssistantService::task_handler(void* pvParameters) {
    VoiceAssistantService* service = static_cast<VoiceAssistantService*>(pvParameters);
    while (true) {
        service->state_machine_tick();
        if (service->current_state != VoiceState::RECORDING && service->current_state != VoiceState::PLAYBACK) {
            vTaskDelay(pdMS_TO_TICKS(10)); 
        }
    }
}

void VoiceAssistantService::state_machine_tick() {
    int16_t audio_buffer[FRAME_SIZE];

    switch (current_state) {
        case VoiceState::WAITING: {
            int samples_read = mic.readFrame(audio_buffer, FRAME_SIZE);
            if (samples_read > 0) {
                int64_t sum = 0; 
                for (int i = 0; i < samples_read; i++) sum += abs(audio_buffer[i]);
                int average_volume = sum / samples_read;

                if (average_volume > VOICE_THRESHOLD) {
                    ESP_LOGI(TAG, "Voice Detected! Vol: %d", average_volume);
                    current_state = VoiceState::RECORDING;
                    recorded_samples_count = 0; 
                    current_silent_samples = 0; 
                }
            }
            break;
        }

        case VoiceState::RECORDING: {
            int samples_read = mic.readFrame(audio_buffer, FRAME_SIZE);
            if (samples_read > 0) {
                int64_t sum = 0; 
                for (int i = 0; i < samples_read; i++) sum += abs(audio_buffer[i]);
                int average_volume = sum / samples_read;

                if (average_volume < SILENCE_THRESHOLD) {
                    current_silent_samples += samples_read;
                } else {
                    current_silent_samples = 0;
                }

                int remaining = MAX_PSRAM_SAMPLES - recorded_samples_count;
                int copy_count = min(samples_read, remaining);
                memcpy(&record_buffer[recorded_samples_count], audio_buffer, copy_count * sizeof(int16_t));
                recorded_samples_count += copy_count;

                int max_silent_samples = (SAMPLE_RATE * SILENCE_TIMEOUT_MS) / 1000;
                
                // --- 录音结束判定 ---
                if (recorded_samples_count >= MAX_PSRAM_SAMPLES || current_silent_samples >= max_silent_samples) {
                    
                    // 1. 剔除末尾多余的静音数据
                    if (current_silent_samples >= max_silent_samples) {
                        recorded_samples_count -= max_silent_samples; 
                    }
                    
                    // 2. 🚀 新增：噪音过滤拦截器 (过滤小于 0.8 秒的声音)
                    float actual_record_seconds = (float)recorded_samples_count / SAMPLE_RATE;
                    if (actual_record_seconds < 0.8f) {
                        ESP_LOGW(TAG, "录音过短 (%.2f 秒)，判定为突发噪音，已忽略。", actual_record_seconds);
                        current_state = VoiceState::WAITING; // 直接放弃，不发给百度，切回等待状态
                    } else {
                        // 正常语音，进入识别流程
                        current_state = VoiceState::PROCESSING_STT;
                    }
                }
            }
            break;
        }

        case VoiceState::PROCESSING_STT: {
            current_text = baidu_api.speech_to_text(record_buffer, recorded_samples_count);
            if (!current_text.empty()) {
                ESP_LOGI(TAG, "=============================");
                ESP_LOGI(TAG, "[User Audio] %s", current_text.c_str());
                ESP_LOGI(TAG, "=============================");
                current_state = VoiceState::PROCESSING_LLM; 
            } else {
                current_state = VoiceState::WAITING;
            }
            break;
        }

        case VoiceState::PROCESSING_LLM: {
            ESP_LOGI(TAG, "思考中... (Asking DeepSeek)");
            
            // 🚀 调用浴火重生的 DeepSeek 大脑！
            std::string llm_response = deepseek_api.ask(current_text);

            if (llm_response.find("Error:") == 0) {
                ESP_LOGE(TAG, "LLM 处理失败，中断对话。");
                current_state = VoiceState::WAITING;
            } else {
                ESP_LOGI(TAG, "=============================");
                ESP_LOGI(TAG, "[DeepSeek回答] %s", llm_response.c_str());
                ESP_LOGI(TAG, "=============================");
                
                // 将回答传递给接下来的 TTS 状态
                current_text = llm_response;
                current_state = VoiceState::PROCESSING_TTS;
            }
            break;
        }

        case VoiceState::PROCESSING_TTS: {
            if (baidu_api.text_to_speech(current_text, record_buffer, MAX_PSRAM_BYTES, downloaded_bytes_count)) {
                ESP_LOGI(TAG, "TTS Downloaded: %d Bytes", downloaded_bytes_count);
                current_state = VoiceState::PLAYBACK;
            } else {
                current_state = VoiceState::WAITING;
            }
            break;
        }

        case VoiceState::PLAYBACK: {
            ESP_LOGI(TAG, "播放声音...");
            
            // ==========================================
            // 🚀 架构师级补丁：动态解析 WAV 头并调整 I2S 时钟
            // ==========================================
            uint8_t* wav_header = (uint8_t*)record_buffer;
            
            // 从 WAV 头部的第 24-27 字节提取真实的采样率 (小端序)
            uint32_t real_sample_rate = wav_header[24] | (wav_header[25] << 8) | 
                                        (wav_header[26] << 16) | (wav_header[27] << 24);
            
            // 如果解析失败或者非标准，默认用回 16000
            if (real_sample_rate < 8000 || real_sample_rate > 48000) {
                real_sample_rate = 16000;
            }
            
            ESP_LOGI(TAG, "侦测到音频真实采样率: %d Hz", real_sample_rate);

            // 动态切换 I2S 硬件时钟！ (假设你底层使用的是 I2S_NUM_0)
            i2s_set_sample_rates(I2S_NUM_0, real_sample_rate);

            int16_t* pcm_start = record_buffer + 22; // 跳过 WAV 44 字节头
            int pcm_samples_to_play = (downloaded_bytes_count - 44) / 2;
            
            // 数字音量控制 (减弱功放声音，可按需修改)
            int volume_divider = 10; 
            for (int i = 0; i < pcm_samples_to_play; i++) {
                pcm_start[i] = pcm_start[i] / volume_divider; 
            }

            int samples_written = 0;
            while (samples_written < pcm_samples_to_play) {
                int chunk = min(FRAME_SIZE, pcm_samples_to_play - samples_written);
                amp.writeFrame(&pcm_start[samples_written], chunk);
                samples_written += chunk;
            }
            
            // 🚀 播放完毕后，极其重要的一步：把采样率切回 16000，否则下次录音麦克风会变“快进”！
            i2s_set_sample_rates(I2S_NUM_0, 16000);
            
            vTaskDelay(pdMS_TO_TICKS(1500)); 
            current_state = VoiceState::WAITING;
            break;
        }
    }
}