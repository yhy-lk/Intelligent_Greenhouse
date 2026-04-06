#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "MCAL/i2s_hal.h"
#include "ECUAL/MAX98357AETE/max98357a_driver.h"
#include "Config/secrets.h"

#include <driver/i2s.h>

// ==========================================
// 全局对象与内存池
// ==========================================
Max98357aDriver amp;
String baidu_access_token = "";

const int MAX_PSRAM_BYTES = 640000; // 约 312KB，足够放十几秒的语音了
int16_t* tts_buffer = nullptr;

// 固定的测试文案
const String TEST_TEXT = "我想起了你，心里就满是甜蜜呢。要是能和你一起在树荫下散步，手牵手，聊聊日常的小事，那该多美好呀，哈哈哈。";

// URL 编码辅助函数
String urlEncode(String str) {
    String encodedString = "";
    char c, code0, code1;
    for (int i = 0; i < str.length(); i++) {
        c = str.charAt(i);
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

// ==========================================
// 初始化网络与鉴权
// ==========================================
void setupNetworkAndToken() {
    Serial.print("\n[Sys] Connecting Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[Sys] Wi-Fi Connected!");

    Serial.println("[Sys] Fetching Baidu Token...");
    HTTPClient http;
    String token_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=" + String(BAIDU_API_KEY) + "&client_secret=" + String(BAIDU_SECRET_KEY);
    
    http.begin(token_url);
    if (http.GET() == HTTP_CODE_OK) {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, http.getString());
        baidu_access_token = doc["access_token"].as<String>();
        Serial.println("[Sys] Token OK!");
    } else {
        Serial.println("[Sys] Token Failed!");
    }
    http.end();
}

// ==========================================
// 核心：合成并播放
// ==========================================
void testTTSVoice(int per_id) {
    if (baidu_access_token == "") {
        Serial.println("[Error] No token!");
        return;
    }

    Serial.printf("\n>>> 正在向百度请求音色 ID: %d ...\n", per_id);
    
    HTTPClient http;
    http.begin("https://tsn.baidu.com/text2audio");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.setTimeout(10000);

    // 动态传入 per_id
    String post_data = "tex=" + urlEncode(TEST_TEXT) + 
                       "&tok=" + baidu_access_token + 
                       "&cuid=esp32_dev&ctp=1&lan=zh&spd=5&pit=5&vol=5" + 
                       "&per=" + String(per_id) + "&aue=6";

    int httpCode = http.POST(post_data);
    int downloaded_bytes = 0;

    if (httpCode == HTTP_CODE_OK || httpCode == 206) {
        String contentType = http.header("Content-Type");
        if (contentType.indexOf("application/json") >= 0) {
            // 如果返回的是 JSON，说明报错了（比如没有大模型音色权限）
            Serial.println("[Error] 百度拒绝了该音色请求 (可能是没有领免费额度):");
            Serial.println(http.getString());
            http.end();
            return;
        }

        int total_size = http.getSize();
        WiFiClient* stream = http.getStreamPtr();
        unsigned long timeout = millis();
        
        while (http.connected() && downloaded_bytes < MAX_PSRAM_BYTES) {
            if (stream->available()) {
                int bytes_read = stream->read((uint8_t*)tts_buffer + downloaded_bytes, MAX_PSRAM_BYTES - downloaded_bytes);
                if (bytes_read > 0) {
                    downloaded_bytes += bytes_read;
                    timeout = millis();
                }
            }
            if (total_size > 0 && downloaded_bytes >= total_size) break;
            if (millis() - timeout > 5000) break;
            delay(1);
        }
        Serial.printf("<<< 音频下载完毕: %d Bytes\n", downloaded_bytes);
        
        // 播放逻辑
        // ==========================================
        // 🚀 架构师级补丁：动态解析 WAV 头并调整 I2S 时钟
        // ==========================================
        uint8_t* wav_header = (uint8_t*)tts_buffer;
        // 从 WAV 头部的第 24-27 字节提取真实的采样率 (小端序)
        uint32_t real_sample_rate = wav_header[24] | (wav_header[25] << 8) | 
                                    (wav_header[26] << 16) | (wav_header[27] << 24);
        
        Serial.printf("[法医鉴定] 百度返回的真实采样率: %d Hz\n", real_sample_rate);

        // 动态将 ESP32 的 I2S 硬件时钟切换到真实采样率！(假设你的 I2SHal 用的是 I2S_NUM_0)
        i2s_set_sample_rates(I2S_NUM_0, real_sample_rate);
        
        Serial.println("[>] 开始原声播放...");
        int16_t* pcm_start = tts_buffer + 22; // 跳过 WAV 44 字节头
        int pcm_samples_to_play = (downloaded_bytes - 44) / 2;
        
        // 音量衰减 (防炸耳)
        int volume_divider = 4; 
        for (int i = 0; i < pcm_samples_to_play; i++) {
            pcm_start[i] = pcm_start[i] / volume_divider; 
        }

        int samples_written = 0;
        while (samples_written < pcm_samples_to_play) {
            int chunk = min(256, pcm_samples_to_play - samples_written);
            amp.writeFrame(&pcm_start[samples_written], chunk);
            samples_written += chunk;
        }
        
        // 播放完毕后，把采样率切回 16000，以免影响麦克风后续的录音！
        i2s_set_sample_rates(I2S_NUM_0, 16000);
        Serial.println("[*] 播放结束。已恢复 16kHz 录音时钟。请输入下一个 ID:");        

    } else {
        Serial.printf("[Error] TTS HTTP 请求失败: %d\n", httpCode);
    }
    http.end();
}

void setup() {
    delay(2000);
    Serial.begin(115200);

    if (!I2SHal::getInstance().begin()) {
        Serial.println("I2S Failed!");
        while(1);
    }

    if (psramFound()) {
        tts_buffer = (int16_t*)ps_malloc(MAX_PSRAM_BYTES);
    }
    if (!tts_buffer) while(1);

    setupNetworkAndToken();

    Serial.println("=========================================");
    Serial.println("🎧 百度 TTS 音色海选控制台已启动！");
    Serial.println("请在下方输入框键入音色 ID（例如 4196）并按回车:");
    Serial.println("推荐测试 ID: 4196(清影), 4197(沁遥), 4150(湘玉)");
    Serial.println("=========================================");
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // 清除空格和换行
        
        if (input.length() > 0) {
            int per_id = input.toInt();
            if (per_id >= 0) {
                testTTSVoice(per_id);
            }
        }
    }
}