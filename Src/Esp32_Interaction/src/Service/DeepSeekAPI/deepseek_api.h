#ifndef DEEPSEEK_API_H
#define DEEPSEEK_API_H

#include <Arduino.h>
#include <stdint.h>

#include <string>
#include <utility>
#include <vector>

#include "deepseek_runtime_config.h"
#include "http_client.h"

struct DeepSeekConfig {
    std::string system_prompt = DEEPSEEK_SYSTEM_PROMPT;
    std::string model = DEEPSEEK_MODEL_NAME;
    float temperature = DEEPSEEK_TEMPERATURE;
    int max_tokens = DEEPSEEK_MAX_TOKENS;
    bool json_mode = false;
    int max_history_messages = DEEPSEEK_MAX_HISTORY_MESSAGES;
    uint8_t status_node_id = DEEPSEEK_STATUS_NODE_ID;
};

class DeepSeekApi {
public:
    DeepSeekApi();

    bool is_connected();

    std::string ask(const std::string& question, const DeepSeekConfig& config = {});
    void clear_history();

    std::string get_last_error() const;
    int get_last_status_code() const;

private:
    std::string last_error;
    int last_status_code;

    std::vector<std::pair<std::string, std::string>> chat_history;
    DeepSeekConfig default_config;
    HttpClient http_client;

    std::string build_status_prompt(const std::string& user_question, const DeepSeekConfig& config) const;
    std::string build_request_json(const std::string& question, const DeepSeekConfig& config);
};

#endif // DEEPSEEK_API_H
