#include "deepseek_api.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include "../../Config/secrets.h"

DeepSeekApi::DeepSeekApi() {
    last_status_code = 0;
    last_error = "";
    default_config = DeepSeekConfig{};
    
    // 初始化你自己的 HTTP 库
    http_client.init(true); 
    http_client.set_timeout(30000); // 30秒硬核超时
    http_client.set_debug_mode(true); 
}

bool DeepSeekApi::connect_to_wifi() {
    Serial.printf("[DeepSeek] Connecting to Wi-Fi %s...\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(500);
        Serial.print(".");
    }
    return WiFi.status() == WL_CONNECTED;
}

bool DeepSeekApi::is_connected() {
    return WiFi.status() == WL_CONNECTED;
}

void DeepSeekApi::clear_history() {
    chat_history.clear();
    Serial.println("[DeepSeek] Chat history cleared.");
}

std::string DeepSeekApi::ask(const std::string& question, const DeepSeekConfig& config) {
    if (!is_connected()) {
        if (!connect_to_wifi()) return "Error: No Wi-Fi";
    }
    
    Serial.printf("[DeepSeek] Free SRAM: %u bytes\n", ESP.getFreeHeap());
    
    std::string json_request = build_request_json(question, config);
    if (json_request.empty()) return "Error: JSON build failed";

    // 准备发送请求
    http_client.clear_headers();
    http_client.set_header("Authorization", "Bearer " + std::string(DEEPSEEK_API_KEY));
    
    Serial.println("[DeepSeek] Sending raw request via Custom HttpClient...");
    
    // 使用你自己的底层库发起 POST
    std::string raw_response = http_client.post(
        "api.deepseek.com", 
        443, 
        "/v1/chat/completions", 
        json_request, 
        "application/json"
    );

    last_status_code = http_client.get_last_status_code();
    std::string result = "";

    if (last_status_code == 200) {
        if (raw_response.empty() || raw_response.find("Error:") == 0) {
            Serial.println("[DeepSeek] 致命错误：底层的 custom http 读回来的数据不对劲！");
            result = raw_response;
        } else {
            // 解析纯净的 JSON (你的 HttpClient 已经帮你完美剔除了 Chunked 头部)
            DynamicJsonDocument doc(8192);
            DeserializationError error = deserializeJson(doc, raw_response.c_str());
            
            if (!error) {
                if (doc.containsKey("error")) {
                    result = std::string("API Error: ") + doc["error"]["message"].as<const char*>();
                } else {
                    const char* answer = doc["choices"][0]["message"]["content"];
                    if (answer) {
                        result = std::string(answer);
                        
                        // 计入记忆
                        chat_history.push_back({"user", question});
                        chat_history.push_back({"assistant", result});
                        while (chat_history.size() > config.max_history_messages) {
                            chat_history.erase(chat_history.begin());
                        }
                    } else {
                        result = "Error: Missing content";
                    }
                }
            } else {
                Serial.printf("[DeepSeek] JSON Parse Failed: %s\n", error.c_str());
                Serial.println("[DeepSeek] Raw JSON Snippet: " + String(raw_response.substr(0, 200).c_str()));
                result = "Error: JSON Parse Failed";
            }
        }
    } else {
        last_error = http_client.get_last_error();
        Serial.printf("[DeepSeek] HTTP Error %d: %s\n", last_status_code, last_error.c_str());
        result = "Error: HTTP " + std::to_string(last_status_code);
    }
    
    return result;
}

std::string DeepSeekApi::build_request_json(const std::string& question, const DeepSeekConfig& config) {
    DynamicJsonDocument doc(4096); 
    
    doc["model"] = config.model.c_str();
    doc["temperature"] = config.temperature;
    doc["max_tokens"] = config.max_tokens;
    doc["stream"] = false; // 明确拒绝流式
    
    JsonArray messages = doc.createNestedArray("messages");
    
    if (!config.system_prompt.empty()) {
        JsonObject sys_msg = messages.createNestedObject();
        sys_msg["role"] = "system";
        sys_msg["content"] = config.system_prompt.c_str();
    }
    
    for (const auto& msg : chat_history) {
        JsonObject h_msg = messages.createNestedObject();
        h_msg["role"] = msg.first.c_str();
        h_msg["content"] = msg.second.c_str();
    }
    
    JsonObject current_msg = messages.createNestedObject();
    current_msg["role"] = "user";
    current_msg["content"] = question.c_str();
    
    std::string json_request;
    serializeJson(doc, json_request);
    return json_request;
}