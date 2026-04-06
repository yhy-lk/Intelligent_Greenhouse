#ifndef WIFI_HAL_H
#define WIFI_HAL_H

#include <Arduino.h>
#include <WiFi.h>
#include <string>

class WiFiHAL {
public:
    WiFiHAL();
    
    // 初始化
    bool init();
    
    // 连接管理
    bool connect(const char* ssid, const char* password, unsigned long timeout_ms = 10000);
    bool disconnect();
    bool isConnected();
    
    // 状态获取
    std::string getIPAddress();
    std::string getSSID();
    int getRSSI();
    
    // 配置
    void setDebugMode(bool enabled);
    
private:
    bool debugMode;
};

#endif // WIFI_HAL_H
