#include "wifi_hal.h"
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


WiFiHAL::WiFiHAL() : debugMode(false) {
}

bool WiFiHAL::init() {
    // WiFi库已经由Arduino框架初始化
    return true;
}

bool WiFiHAL::connect(const char* ssid, const char* password, unsigned long timeout_ms) {
    if (debugMode) {
        Serial.println("Connecting to Wi-Fi...");
        Serial.print("SSID: ");
        Serial.println(ssid);
    }
    
    WiFi.begin(ssid, password);
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout_ms) {
        vTaskDelay(pdMS_TO_TICKS(500));
        if (debugMode) {
            Serial.print(".");
        }
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        if (debugMode) {
            Serial.println("\nWi-Fi connected successfully!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
        }
        return true;
    } else {
        if (debugMode) {
            Serial.println("\nWi-Fi connection failed!");
        }
        return false;
    }
}

bool WiFiHAL::disconnect() {
    WiFi.disconnect();
    if (debugMode) {
        Serial.println("Wi-Fi disconnected");
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
