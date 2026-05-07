#include "deepseek_api.h"

#include <ArduinoJson.h>
#include <WiFi.h>

#include <cstdio>

#include "../../Config/secrets.h"
#include "../SensorState/sensor_state.h"
#include "../WifiService/wifi_service.h"
#include "esp_log.h"

static const char* TAG = "DEEPSEEK_API";

namespace {

void append_sensor_line(std::string& output, const char* label, float value, const char* unit, bool& has_sensor_data) {
    if (value == INVALID_VALUE) {
        return;
    }

    char buffer[96];
    std::snprintf(buffer, sizeof(buffer), "%s%.2f%s", label, value, unit);

    if (!output.empty()) {
        output += "，";
    }

    output += buffer;
    has_sensor_data = true;
}

void append_integer_sensor_line(std::string& output, const char* label, float value, const char* unit,
                                bool& has_sensor_data) {
    if (value == INVALID_VALUE) {
        return;
    }

    char buffer[96];
    std::snprintf(buffer, sizeof(buffer), "%s%.0f%s", label, value, unit);

    if (!output.empty()) {
        output += "，";
    }

    output += buffer;
    has_sensor_data = true;
}

} // namespace

DeepSeekApi::DeepSeekApi() {
    last_status_code = 0;
    last_error.clear();
    default_config = DeepSeekConfig{};

    http_client.init(true);
    http_client.set_timeout(30000);
    http_client.set_debug_mode(true);
}

bool DeepSeekApi::is_connected() {
    return WiFi.status() == WL_CONNECTED;
}

void DeepSeekApi::clear_history() {
    chat_history.clear();
    ESP_LOGI(TAG, "Chat history cleared.");
}

std::string DeepSeekApi::get_last_error() const {
    return last_error;
}

int DeepSeekApi::get_last_status_code() const {
    return last_status_code;
}

std::string DeepSeekApi::build_status_prompt(const std::string& user_question, const DeepSeekConfig& config) const {
    NodeData snapshot;
    const bool snapshot_ok = sensor_state_get_node_snapshot(config.status_node_id, &snapshot);
    const bool is_online = sensor_state_is_online(config.status_node_id);

    std::string sensor_summary;
    bool has_sensor_data = false;

    if (snapshot_ok) {
        append_sensor_line(sensor_summary, "温度", snapshot.current_values[PARAM_IDX_TEMPERATURE], "摄氏度",
                           has_sensor_data);
        append_sensor_line(sensor_summary, "空气湿度", snapshot.current_values[PARAM_IDX_HUMIDITY_AIR], "百分比",
                           has_sensor_data);
        append_sensor_line(sensor_summary, "土壤湿度", snapshot.current_values[PARAM_IDX_HUMIDITY_SOIL], "百分比",
                           has_sensor_data);
        append_integer_sensor_line(sensor_summary, "光照强度", snapshot.current_values[PARAM_IDX_LIGHT_INTENSITY], "勒克斯",
                                   has_sensor_data);
        append_integer_sensor_line(sensor_summary, "二氧化碳浓度", snapshot.current_values[PARAM_IDX_CO2_LEVEL], "ppm",
                                   has_sensor_data);
        append_integer_sensor_line(sensor_summary, "水位", snapshot.current_values[PARAM_IDX_WATER_LEVEL], "百分比",
                                   has_sensor_data);
        append_integer_sensor_line(sensor_summary, "风扇转速", snapshot.current_values[PARAM_IDX_FAN_SPEED], "转每分钟",
                                   has_sensor_data);
    }

    std::string prompt = "请基于以下温室实时状态先做简短点评，再结合用户问题回答。";
    prompt += "当前节点编号为";
    prompt += std::to_string(config.status_node_id);
    prompt += "。";

    if (is_online) {
        prompt += "节点在线。";
    } else {
        prompt += "节点当前离线或刚启动。";
    }

    if (has_sensor_data) {
        prompt += "当前可用传感器数据为";
        prompt += sensor_summary;
        prompt += "。";
    } else {
        prompt += "当前没有可用的传感器数据。";
    }

    prompt += "用户问题是";
    prompt += user_question;
    prompt += "。";

    return prompt;
}

std::string DeepSeekApi::ask(const std::string& question, const DeepSeekConfig& config) {
    if (!is_connected()) {
        if (!WifiService::getInstance().connect()) {
            return "Error: No Wi-Fi";
        }
    }

    ESP_LOGI(TAG, "Free SRAM: %u bytes", ESP.getFreeHeap());

    const std::string prompt = build_status_prompt(question, config);
    ESP_LOGI(TAG, "DeepSeek prompt: %s", prompt.c_str());
    const std::string json_request = build_request_json(prompt, config);
    if (json_request.empty()) {
        return "Error: JSON build failed";
    }

    http_client.clear_headers();
    http_client.set_header("Authorization", "Bearer " + std::string(DEEPSEEK_API_KEY));

    ESP_LOGI(TAG, "Sending raw request via Custom HttpClient...");

    const std::string raw_response = http_client.post(
        "api.deepseek.com",
        443,
        "/v1/chat/completions",
        json_request,
        "application/json"
    );

    last_status_code = http_client.get_last_status_code();
    std::string result;

    if (last_status_code == 200) {
        if (raw_response.empty() || raw_response.find("Error:") == 0) {
            ESP_LOGE(TAG, "Custom HTTP client returned an invalid body.");
            result = raw_response;
        } else {
            DynamicJsonDocument doc(8192);
            DeserializationError error = deserializeJson(doc, raw_response.c_str());

            if (!error) {
                if (doc.containsKey("error")) {
                    result = std::string("API Error: ") + doc["error"]["message"].as<const char*>();
                } else {
                    const char* answer = doc["choices"][0]["message"]["content"];
                    if (answer != nullptr) {
                        result = std::string(answer);

                        chat_history.push_back({"user", question});
                        chat_history.push_back({"assistant", result});
                        while ((int)chat_history.size() > config.max_history_messages) {
                            chat_history.erase(chat_history.begin());
                        }
                    } else {
                        result = "Error: Missing content";
                    }
                }
            } else {
                ESP_LOGE(TAG, "JSON Parse Failed: %s", error.c_str());
                ESP_LOGE(TAG, "Raw JSON Snippet: %s", raw_response.substr(0, 200).c_str());
                result = "Error: JSON Parse Failed";
            }
        }
    } else {
        last_error = http_client.get_last_error();
        ESP_LOGE(TAG, "HTTP Error %d: %s", last_status_code, last_error.c_str());
        result = "Error: HTTP " + std::to_string(last_status_code);
    }

    return result;
}

std::string DeepSeekApi::build_request_json(const std::string& question, const DeepSeekConfig& config) {
    DynamicJsonDocument doc(4096);

    doc["model"] = config.model.c_str();
    doc["temperature"] = config.temperature;
    doc["max_tokens"] = config.max_tokens;
    doc["stream"] = false;

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
