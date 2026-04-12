#include "baidu_api.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_log.h" // 引入ESP-IDF日志宏

// 定义当前文件的日志标签
static const char *TAG = "BAIDU_API";

BaiduApi::BaiduApi() : access_token("") {}

bool BaiduApi::init(const std::string& api_key, const std::string& secret_key) {
    if (WiFi.status() != WL_CONNECTED) {
        ESP_LOGE(TAG, "Wi-Fi not connected!");
        return false;
    }

    ESP_LOGI(TAG, "Fetching Token...");
    HTTPClient http;
    String token_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=";
    token_url += api_key.c_str();
    token_url += "&client_secret=";
    token_url += secret_key.c_str();

    http.begin(token_url);
    int httpCode = http.GET();
    bool success = false;
    
    if (httpCode > 0) { // HTTP 成功连上服务器
        String payload = http.getString(); // 安全接收字符串
        
        if (httpCode == HTTP_CODE_OK) {
            DynamicJsonDocument doc(2048);
            DeserializationError error = deserializeJson(doc, payload);
            
            if (!error) {
                if (doc.containsKey("access_token")) {
                    access_token = doc["access_token"].as<std::string>();
                    ESP_LOGI(TAG, "Token fetched successfully!");
                    success = true;
                } else {
                    ESP_LOGW(TAG, "API returned 200, but no access_token found!");
                    // 原始响应可能较长，使用 Debug 级别输出避免刷屏
                    ESP_LOGD(TAG, "Raw Response: %s", payload.c_str());
                }
            } else {
                ESP_LOGE(TAG, "JSON Parse failed: %s", error.c_str());
                ESP_LOGD(TAG, "Raw Payload: %s", payload.c_str());
            }
        } else {
            ESP_LOGE(TAG, "Failed to get token, HTTP error: %d", httpCode);
            ESP_LOGD(TAG, "Response: %s", payload.c_str());
        }
    } else {
        ESP_LOGE(TAG, "HTTP Connection failed, error: %s", http.errorToString(httpCode).c_str());
    }
    
    http.end();
    return success;
}

std::string BaiduApi::speech_to_text(int16_t* audio_data, int sample_count) {
    if (access_token.empty()) return "";

    ESP_LOGI(TAG, "Uploading %.2f seconds of audio to STT...", (float)sample_count / 16000.0);
    
    HTTPClient http;
    String url = "http://vop.baidu.com/server_api?dev_pid=1537&cuid=esp32_device&token=";
    url += access_token.c_str();

    http.begin(url);
    http.addHeader("Content-Type", "audio/pcm;rate=16000");

    int httpCode = http.POST((uint8_t*)audio_data, sample_count * 2);
    std::string recognized_text = "";

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error && doc["err_no"] == 0) {
            recognized_text = doc["result"][0].as<std::string>();
        } else {
            ESP_LOGE(TAG, "STT API Error: %s", payload.c_str());
        }
    } else {
        ESP_LOGE(TAG, "STT HTTP POST Failed, code: %d", httpCode);
    }
    
    http.end();
    return recognized_text;
}

bool BaiduApi::text_to_speech(const std::string& text, int16_t* buffer, int max_bytes, int& out_downloaded_bytes) {
    if (access_token.empty()) return false;

    ESP_LOGI(TAG, "Downloading TTS Voice...");
    HTTPClient http;
    http.begin("https://tsn.baidu.com/text2audio");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String post_data = "tex=";
    post_data += url_encode(text).c_str();
    post_data += "&tok=";
    post_data += access_token.c_str();
    post_data += "&cuid=esp32_dev&ctp=1&lan=zh&spd=5&pit=5&vol=5&per=4196&aue=6";

    int httpCode = http.POST(post_data);
    out_downloaded_bytes = 0;
    bool success = false;

    if (httpCode == HTTP_CODE_OK || httpCode == 206) {
        int total_size = http.getSize(); 
        WiFiClient* stream = http.getStreamPtr();
        unsigned long timeout = millis();
        
        while (http.connected() && out_downloaded_bytes < max_bytes) {
            if (stream->available()) {
                int bytes_read = stream->read((uint8_t*)buffer + out_downloaded_bytes, max_bytes - out_downloaded_bytes);
                if (bytes_read > 0) {
                    out_downloaded_bytes += bytes_read;
                    timeout = millis();
                }
            } 
            if (total_size > 0 && out_downloaded_bytes >= total_size) {
                success = true;
                break;
            }
            if (millis() - timeout > 5000) {
                ESP_LOGE(TAG, "TTS Download timeout!");
                break; 
            }
            delay(1); 
        }
        success = (out_downloaded_bytes > 0);
    } else {
        ESP_LOGE(TAG, "TTS Failed: %d", httpCode);
    }
    http.end();
    return success;
}

std::string BaiduApi::url_encode(const std::string& str) {
    std::string encodedString = "";
    char c, code0, code1;
    for (size_t i = 0; i < str.length(); i++) {
        c = str[i];
        if (c == ' ') { encodedString += '+'; } 
        else if (isalnum(c)) { encodedString += c; } 
        else {
            code1 = (c & 0xf) + '0';
            if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
            c = (c >> 4) & 0xf;
            code0 = c + '0';
            if (c > 9) code0 = c - 10 + 'A';
            encodedString += '%';
            encodedString += code0;
            encodedString += code1;
        }
    }
    return encodedString;
}