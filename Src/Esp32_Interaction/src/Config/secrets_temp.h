#pragma once

#include <string>
#include <vector>
#include "wifi_core_types.h"

// ============================================================
// Wi-Fi 配置（多组凭证，自动逐个尝试连接）
// 新增/删除环境时，只需在此处增减条目即可
// ============================================================
static const std::vector<WifiCredential> WIFI_CREDENTIALS = {
    {"YOUR_WIFI_SSID", "YOUR_WIFI_PASSWORD"},
};

// DeepSeek API配置
#define DEEPSEEK_API_KEY "YOUR_API_KEY"
#define DEEPSEEK_API_URL "YOUR_API_URL"
#define DEEPSEEK_MODEL "YOUR_MODEL_NAME"

#define BAIDU_API_KEY "YOUR_BAIDU_API_KEY"
#define BAIDU_SECRET_KEY "YOUR_BAIDU_SECRET_KEY"