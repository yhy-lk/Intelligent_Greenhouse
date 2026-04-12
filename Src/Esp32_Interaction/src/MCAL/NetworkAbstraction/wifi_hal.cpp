#include "wifi_hal.h"
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h" // 引入ESP-IDF日志宏

// 定义当前文件的日志标签
static const char *TAG = "WIFI_HAL";

WiFiHAL::WiFiHAL() : debugMode(false) {
}

bool WiFiHAL::init() {
    // WiFi库已经由Arduino框架初始化
    return true;
}

bool WiFiHAL::connect(const char* ssid, const char* password, unsigned long timeout_ms) {
    if (debugMode) {
        // 合并原有的多次打印，使用 ESP_LOGI 输出连接信息
        ESP_LOGI(TAG, "Connecting to Wi-Fi... SSID: %s", ssid);
    }
    
    WiFi.begin(ssid, password);
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout_ms) {
        vTaskDelay(pdMS_TO_TICKS(500));
        if (debugMode) {
            // ESP_LOGx 宏自带换行，不再适合使用连续打印 "." 的方式
            // 改为使用 Debug 级别输出等待状态，避免刷屏干扰常规 Info 日志
            ESP_LOGD(TAG, "Waiting for Wi-Fi connection...");
        }
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        if (debugMode) {
            // 连接成功属于重要状态变更，使用 ESP_LOGI
            ESP_LOGI(TAG, "Wi-Fi connected successfully! IP address: %s", WiFi.localIP().toString().c_str());
        }
        return true;
    } else {
        if (debugMode) {
            // 连接失败属于错误，使用 ESP_LOGE
            ESP_LOGE(TAG, "Wi-Fi connection failed!");
        }
        return false;
    }
}

bool WiFiHAL::disconnect() {
    WiFi.disconnect();
    if (debugMode) {
        // 断开连接属于常规状态变更，使用 ESP_LOGI
        ESP_LOGI(TAG, "Wi-Fi disconnected");
    }
    return true;
}

bool WiFiHAL::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

std::string WiFiHAL::getIPAddress() {
    return std::string(WiFi.localIP().toString().c_str());
}

std::string WiFiHAL::getSSID() {
    return std::string(WiFi.SSID().c_str());
}

int WiFiHAL::getRSSI() {
    return WiFi.RSSI();
}

void WiFiHAL::setDebugMode(bool enabled) {
    debugMode = enabled;
}