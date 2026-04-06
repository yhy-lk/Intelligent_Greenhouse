#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <vector>
#include <string>

class HttpClient {
public:
    HttpClient();
    
    // 初始化
    void init(bool insecure = true);
    
    // HTTP请求
    std::string post(const std::string& url, const std::string& data, const std::string& content_type = "application/json");
    std::string post(const std::string& host, uint16_t port, const std::string& path, 
                const std::string& data, const std::string& content_type = "application/json");
    
    // 头部管理
    void set_header(const std::string& name, const std::string& value);
    void clear_headers();
    
    // 配置
    void set_timeout(unsigned long timeout_ms);
    void set_debug_mode(bool enabled);
    
    // 状态
    int get_last_status_code() const;
    std::string get_last_error() const;
    
private:
    struct Header {
        std::string name;
        std::string value;
    };
    
    WiFiClientSecure client;
    std::vector<Header> headers;
    unsigned long timeout;
    bool debug_mode;
    bool insecure_mode;
    int last_status_code;
    std::string last_error;
    
    std::string read_chunked_response(WiFiClientSecure& client, unsigned long timeout_ms);
    std::string read_content_length_response(WiFiClientSecure& client, int content_length, unsigned long timeout_ms);
};

#endif // HTTP_CLIENT_H
