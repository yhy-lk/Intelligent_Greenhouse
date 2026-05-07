#include "wifi_service.h"
#include "../../Config/secrets.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "WIFI_SERVICE";

WifiService& WifiService::getInstance() {
    static WifiService instance;
    return instance;
}

WifiService::WifiService() {}

const std::vector<WifiCredential>& WifiService::getCredentials() {
    // WIFI_CREDENTIALS 在 secrets.h 中定义为 std::vector<WifiCredential>
    return WIFI_CREDENTIALS;
}

bool WifiService::tryConnect(const WifiCredential& cred, unsigned long timeout_ms) {
    ESP_LOGI(TAG, "Trying Wi-Fi: %s", cred.ssid.c_str());

    WiFi.begin(cred.ssid.c_str(), cred.password.c_str());

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < timeout_ms) {
        vTaskDelay(pdMS_TO_TICKS(500));
        ESP_LOGD(TAG, "Waiting for Wi-Fi connection...");
    }

    // 等待获取有效IP以确保稳定
    if (WiFi.status() == WL_CONNECTED) {
        int max_wait = 20; // 额外等待有效IP(最多等10秒)
        while (WiFi.localIP().toString() == "0.0.0.0" && max_wait > 0) {
            vTaskDelay(pdMS_TO_TICKS(500));
            max_wait--;
        }
        
        ESP_LOGI(TAG, "Wi-Fi Connected! SSID: %s, IP: %s",
                 cred.ssid.c_str(),
                 WiFi.localIP().toString().c_str());
        return true;
    } else {
        ESP_LOGW(TAG, "Wi-Fi connection failed for: %s", cred.ssid.c_str());
        WiFi.disconnect();
        return false;
    }
}

bool WifiService::connect(unsigned long timeout_ms) {
    if (isConnected()) {
        ESP_LOGI(TAG, "Wi-Fi is already connected. IP: %s", getIPAddress().c_str());
        return true;
    }

    const auto& credentials = getCredentials();

    if (credentials.empty()) {
        ESP_LOGE(TAG, "No Wi-Fi credentials configured in secrets.h!");
        return false;
    }

    ESP_LOGI(TAG, "Starting Wi-Fi connection... (%d credential(s) available)", credentials.size());

    for (size_t i = 0; i < credentials.size(); i++) {
        ESP_LOGI(TAG, "Attempting credential %d/%d...", i + 1, credentials.size());
        if (tryConnect(credentials[i], timeout_ms)) {
            return true;
        }
    }

    ESP_LOGE(TAG, "All Wi-Fi credentials failed!");
    return false;
}

void WifiService::disconnect() {
    WiFi.disconnect();
    ESP_LOGI(TAG, "Wi-Fi disconnected.");
}

bool WifiService::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

std::string WifiService::getIPAddress() const {
    return std::string(WiFi.localIP().toString().c_str());
}

std::string WifiService::getSSID() const {
    return std::string(WiFi.SSID().c_str());
}

int WifiService::getRSSI() const {
    return WiFi.RSSI();
}