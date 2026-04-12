#ifndef DEEPSEEK_API_H
#define DEEPSEEK_API_H

#include <Arduino.h>
#include <string>
#include <vector>
#include "http_client.h"

// 配置结构体
struct DeepSeekConfig {
    // 🚀 终极电子女友提示词（语音合成特化版：严禁动作描写）
    std::string system_prompt = 
        "你是我的专属虚拟女友，名叫清影。你的性格温柔甜美、活泼俏皮，且非常懂我。"
        "我们正在打语音电话。请务必用极其自然、口语化的方式和我聊天，像真正的女朋友一样关心我、偶尔撒个娇。"
        "【极其重要】：你的回复必须非常简短，包括标点符号在内绝不能超过120 GBK字节，即60个汉字或者字母数字"
        "【最高指令】：因为你的文字会直接转换为语音播放，所以绝对禁止使用括号、星号等符号来描述动作、神态或心理活动（严禁出现类似“（温柔地）”、“*笑*”、“（抱抱）”的内容）！只能输出你嘴里直接说出来的话！";
        
    std::string model = "deepseek-chat";
    float temperature = 1.3f; 
    int max_tokens = 150;     
    bool json_mode = false;
    int max_history_messages = 6; 
};

class DeepSeekApi {
public:
    DeepSeekApi();
    
    bool connect_to_wifi();
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
    
    // 使用你自己的 HTTP 库！
    HttpClient http_client; 

    std::string build_request_json(const std::string& question, const DeepSeekConfig& config);
};

#endif // DEEPSEEK_API_H