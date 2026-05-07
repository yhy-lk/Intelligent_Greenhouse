#include "http_client.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <string>

#include "esp_log.h"
#include "utils.h"

using namespace Utils;

static const char* TAG = "MyHttp";

namespace {

bool wait_for_available(WiFiClientSecure& client, unsigned long timeout_ms) {
    const unsigned long start_time = millis();
    while (!client.available() && client.connected() && (millis() - start_time < timeout_ms)) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    return client.available() > 0;
}

} // namespace

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
    client.setTimeout(timeout_ms);
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

int HttpClient::get_last_status_code() const {
    return last_status_code;
}

std::string HttpClient::get_last_error() const {
    return last_error;
}

std::string HttpClient::post(const std::string& url, const std::string& data, const std::string& content_type) {
    (void)url;
    (void)data;
    (void)content_type;
    return "Error: URL parsing not implemented in this simplified version";
}

std::string HttpClient::post(const std::string& host, uint16_t port, const std::string& path,
                             const std::string& data, const std::string& content_type) {
    last_error.clear();
    last_status_code = 0;

    if (debug_mode) {
        ESP_LOGD(TAG, "Connecting to %s:%u...", host.c_str(), port);
    }

    if (!client.connect(host.c_str(), port)) {
        last_error = "Connection failed";
        if (debug_mode) {
            ESP_LOGE(TAG, "Connection to server failed.");
        }
        return "Error: " + last_error;
    }

    if (debug_mode) {
        ESP_LOGD(TAG, "Connected! Sending request...");
    }

    std::string request = "POST " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Content-Type: " + content_type + "\r\n";
    request += "Content-Length: " + std::to_string(data.length()) + "\r\n";

    for (const auto& header : headers) {
        request += header.name + ": " + header.value + "\r\n";
    }

    request += "Connection: close\r\n\r\n";
    request += data;

    client.print(request.c_str());

    if (!wait_for_available(client, timeout)) {
        last_error = "No response from server (timeout)";
        client.stop();
        return "Error: " + last_error;
    }

    std::string status_line = client.readStringUntil('\n').c_str();
    if (debug_mode) {
        ESP_LOGD(TAG, "Status: %s", status_line.c_str());
    }

    if (startsWith(status_line, "HTTP/")) {
        const size_t first_space = status_line.find(' ');
        const size_t second_space = status_line.find(' ', first_space + 1);
        if (first_space != std::string::npos && second_space != std::string::npos) {
            last_status_code = std::stoi(status_line.substr(first_space + 1, second_space - first_space - 1));
        }
    }

    bool is_chunked = false;
    int content_length = -1;

    while (client.connected()) {
        if (!wait_for_available(client, timeout)) {
            break;
        }

        std::string line = trim(client.readStringUntil('\n').c_str());
        if (line.empty()) {
            break;
        }

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
    response.reserve(4096);

    if (is_chunked) {
        response = read_chunked_response(client, timeout);
    } else {
        response = read_content_length_response(client, content_length, timeout);
    }

    client.stop();
    return response;
}

std::string HttpClient::read_chunked_response(WiFiClientSecure& client, unsigned long timeout_ms) {
    std::string response;
    uint8_t buffer[256];

    while (client.connected() || client.available()) {
        if (!wait_for_available(client, timeout_ms)) {
            ESP_LOGE(TAG, "Timeout waiting for chunk size.");
            break;
        }

        std::string chunk_size_line = trim(client.readStringUntil('\n').c_str());
        if (chunk_size_line.empty()) {
            continue;
        }

        const int chunk_size = static_cast<int>(strtol(chunk_size_line.c_str(), nullptr, 16));
        if (chunk_size <= 0) {
            break;
        }

        int bytes_read = 0;
        while (bytes_read < chunk_size) {
            if (!wait_for_available(client, timeout_ms)) {
                ESP_LOGE(TAG, "Timeout waiting for chunk body.");
                return response;
            }

            const int to_read = std::min<int>(sizeof(buffer), chunk_size - bytes_read);
            const int available_bytes = client.available();
            const int read_len = std::min(to_read, available_bytes);
            const int read_now = client.read(buffer, read_len);

            if (read_now > 0) {
                response.append(reinterpret_cast<const char*>(buffer), read_now);
                bytes_read += read_now;
                taskYIELD();
            }
        }

        if (!wait_for_available(client, timeout_ms)) {
            ESP_LOGE(TAG, "Timeout waiting for chunk terminator.");
            return response;
        }
        client.readStringUntil('\n');
    }

    return response;
}

std::string HttpClient::read_content_length_response(WiFiClientSecure& client, int content_length, unsigned long timeout_ms) {
    std::string response;
    uint8_t buffer[256];
    int bytes_read = 0;

    const bool has_known_length = content_length > 0;

    while (client.connected() || client.available()) {
        if (has_known_length && bytes_read >= content_length) {
            break;
        }

        if (!wait_for_available(client, timeout_ms)) {
            if (!client.connected() && !client.available()) {
                break;
            }
            ESP_LOGE(TAG, "Read timeout.");
            break;
        }

        int to_read = static_cast<int>(sizeof(buffer));
        if (has_known_length) {
            to_read = std::min(to_read, content_length - bytes_read);
        }

        const int available_bytes = client.available();
        const int read_len = std::min(to_read, available_bytes);
        const int read_now = client.read(buffer, read_len);

        if (read_now > 0) {
            response.append(reinterpret_cast<const char*>(buffer), read_now);
            bytes_read += read_now;
            taskYIELD();
        }
    }

    return response;
}
