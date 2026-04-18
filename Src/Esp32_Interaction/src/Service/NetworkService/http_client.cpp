#include "http_client.h"
#include <algorithm>
#include <string>
#include "utils.h"
#include "esp_log.h" // 引入 ESP-IDF 日志库

// 假设你的 utils 里有 trim, startsWith, indexOf。如果没有，请确保它们能正常工作。
using namespace Utils;

// 定义日志标签
static const char *TAG = "MyHttp";

HttpClient::HttpClient() 
    : timeout(30000), debug_mode(true), insecure_mode(true), last_status_code(0) {
}

void HttpClient::init(bool insecure) {
    insecure_mode = insecure;
    if (insecure_mode) {
        client.setInsecure();
    }
}

void HttpClient::set_timeout(unsigned long timeout_ms) {
    timeout = timeout_ms;
    client.setTimeout(timeout_ms); // 设置底层 TLS 超时
}

void HttpClient::set_debug_mode(bool enabled) {
    debug_mode = enabled;
}

void HttpClient::set_header(const std::string& name, const std::string& value) {
    headers.push_back({name, value});
}

void HttpClient::clear_headers() {
    headers.clear();
}

int HttpClient::get_last_status_code() const { return last_status_code; }
std::string HttpClient::get_last_error() const { return last_error; }

std::string HttpClient::post(const std::string& url, const std::string& data, const std::string& content_type) {
    return "Error: URL parsing not implemented in this simplified version";
}

std::string HttpClient::post(const std::string& host, uint16_t port, const std::string& path, 
                             const std::string& data, const std::string& content_type) {
    last_error = "";
    last_status_code = 0;
    
    if (debug_mode) {
        ESP_LOGD(TAG, "Connecting to %s:%d...", host.c_str(), port);
    }
    
    if (!client.connect(host.c_str(), port)) {
        last_error = "Connection failed";
        if (debug_mode) ESP_LOGE(TAG, "Connection to server failed!");
        return "Error: " + last_error;
    }
    
    if (debug_mode) ESP_LOGD(TAG, "Connected! Sending request...");
    
    // 构建请求
    std::string request = "POST " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Content-Type: " + content_type + "\r\n";
    request += "Content-Length: " + std::to_string(data.length()) + "\r\n";
    
    for (const auto& header : headers) {
        request += header.name + ": " + header.value + "\r\n";
    }
    
    request += "Connection: close\r\n\r\n";
    request += data;
    
    // 发送全部数据
    client.print(request.c_str());
    
    // 等待响应
    unsigned long start_time = millis();
    while (!client.available() && millis() - start_time < timeout) {
        delay(10);
    }
    
    if (!client.available()) {
        last_error = "No response from server (timeout)";
        client.stop();
        return "Error: " + last_error;
    }
    
    // 读取状态行
    std::string status_line = client.readStringUntil('\n').c_str();
    if (debug_mode) ESP_LOGD(TAG, "Status: %s", status_line.c_str());
    
    // 解析状态码
    if (startsWith(status_line, "HTTP/")) {
        size_t first_space = status_line.find(' ');
        size_t second_space = status_line.find(' ', first_space + 1);
        if (first_space != std::string::npos && second_space != std::string::npos) {
            last_status_code = std::stoi(status_line.substr(first_space + 1, second_space - first_space - 1));
        }
    }
    
    // 读取头部
    bool is_chunked = false;
    int content_length = -1;
    
    while (client.available() || client.connected()) {
        std::string line = client.readStringUntil('\n').c_str();
        line = trim(line);
        if (line.empty()) break; // 头部结束
        
        // 转换为小写比较更安全
        std::string line_lower = line;
        std::transform(line_lower.begin(), line_lower.end(), line_lower.begin(), ::tolower);
        
        if (startsWith(line_lower, "transfer-encoding:") && indexOf(line_lower, "chunked") >= 0) {
            is_chunked = true;
        } else if (startsWith(line_lower, "content-length:")) {
            content_length = std::stoi(line.substr(15));
        }
    }
    
    if (debug_mode) {
        ESP_LOGD(TAG, "Is Chunked: %s, Content-Length: %d", is_chunked ? "Yes" : "No", content_length);
    }

    std::string response;
    // 预分配内存，极大减少内存碎片化
    response.reserve(4096); 
    
    if (is_chunked) {
        response = read_chunked_response(client, timeout);
    } else {
        response = read_content_length_response(client, content_length, timeout);
    }
    
    client.stop();
    return response;
}

// 🚀 核心优化：块读取算法，代替原来的单字节读取
std::string HttpClient::read_chunked_response(WiFiClientSecure& client, unsigned long timeout_ms) {
    std::string response;
    unsigned long start_time = millis();
    uint8_t buffer[256]; 
    
    while (client.connected() || client.available()) {
        std::string chunk_size_line = client.readStringUntil('\n').c_str();
        chunk_size_line = trim(chunk_size_line);
        if (chunk_size_line.empty()) continue;
        
        int chunk_size = static_cast<int>(strtol(chunk_size_line.c_str(), NULL, 16));
        if (chunk_size == 0) break; // Chunk结束
        
        int bytes_read = 0;
        while (bytes_read < chunk_size && (client.connected() || client.available())) {
            int to_read = std::min((int)sizeof(buffer), chunk_size - bytes_read);
            // 确保缓冲区有数据
            while (!client.available() && (millis() - start_time < timeout_ms)) { delay(1); }
            
            if (client.available()) {
                int r = client.read(buffer, std::min(to_read, client.available()));
                if (r > 0) {
                    response.append((char*)buffer, r);
                    bytes_read += r;
                }
            }
            if (millis() - start_time > timeout_ms) break;
        }
        
        // 读取 trailing CRLF
        while (!client.available() && (millis() - start_time < timeout_ms)) { delay(1); }
        if (client.available()) client.readStringUntil('\n');
        
        if (millis() - start_time > timeout_ms) {
            ESP_LOGE(TAG, "Chunked read timeout!");
            break;
        }
    }
    return response;
}

// 🚀 核心优化：块读取
std::string HttpClient::read_content_length_response(WiFiClientSecure& client, int content_length, unsigned long timeout_ms) {
    std::string response;
    unsigned long start_time = millis();
    uint8_t buffer[256];
    
    int bytes_read = 0;
    // 如果没有 content_length，就一直读到断开
    int target_length = (content_length > 0) ? content_length : 999999; 
    
    while (bytes_read < target_length && (client.connected() || client.available())) {
        while (!client.available() && client.connected() && (millis() - start_time < timeout_ms)) { delay(1); }
        
        if (client.available()) {
            int to_read = std::min((int)sizeof(buffer), target_length - bytes_read);
            int r = client.read(buffer, std::min(to_read, client.available()));
            if (r > 0) {
                response.append((char*)buffer, r);
                bytes_read += r;
            }
        }
        if (millis() - start_time > timeout_ms) {
            ESP_LOGE(TAG, "Read timeout!");
            break;
        }
    }
    return response;
}