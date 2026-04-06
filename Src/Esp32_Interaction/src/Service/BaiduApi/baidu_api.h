#pragma once
#include <Arduino.h>
#include <string>

class BaiduApi {
public:
    BaiduApi();
    ~BaiduApi() = default;

    // 初始化并获取 Token
    bool init(const std::string& api_key, const std::string& secret_key);
    
    // 语音转文字 (STT)
    std::string speech_to_text(int16_t* audio_data, int sample_count);
    
    // 文字转语音 (TTS)，直接将音频流下载到提供的 PSRAM 缓冲区中
    bool text_to_speech(const std::string& text, int16_t* buffer, int max_bytes, int& out_downloaded_bytes);

private:
    std::string access_token;
    std::string url_encode(const std::string& str);
};