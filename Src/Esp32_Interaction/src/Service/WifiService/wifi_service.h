#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <string>
#include <vector>
#include <utility>

#include "esp_log.h"
#include "../../Config/wifi_core_types.h"

/**
 * @brief WiFi 自动连接服务
 * 
 * 内置多组 WiFi 凭证，启动时逐个尝试连接，
 * 换环境时只需在 secrets.h 中增减凭证即可，无需修改业务代码。
 */
class WifiService {
public:
    /**
     * @brief 获取单例实例
     */
    static WifiService& getInstance();

    /**
     * @brief 初始化并连接 WiFi
     * 
     * 遍历内置凭证列表，逐个尝试连接，直到成功。
     * 
     * @param timeout_ms 每组凭证的连接超时时间（毫秒），默认 10000ms
     * @return true  连接成功
     * @return false 所有凭证均连接失败
     */
    bool connect(unsigned long timeout_ms = 10000);

    /**
     * @brief 断开 WiFi 连接
     */
    void disconnect();

    /**
     * @brief 当前是否已连接
     */
    bool isConnected() const;

    /**
     * @brief 获取当前连接的 IP 地址
     */
    std::string getIPAddress() const;

    /**
     * @brief 获取当前连接的 SSID
     */
    std::string getSSID() const;

    /**
     * @brief 获取当前信号强度 (RSSI)
     */
    int getRSSI() const;

private:
    WifiService();
    ~WifiService() = default;

    // 禁止拷贝和赋值
    WifiService(const WifiService&) = delete;
    WifiService& operator=(const WifiService&) = delete;

    /**
     * @brief 尝试使用单组凭证连接
     */
    bool tryConnect(const WifiCredential& cred, unsigned long timeout_ms);

    /**
     * @brief 获取所有 WiFi 凭证
     * 
     * 凭证来源于 secrets.h 中定义的 WIFI_CREDENTIALS 列表。
     */
    static const std::vector<WifiCredential>& getCredentials();
};