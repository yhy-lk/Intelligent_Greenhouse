#ifndef BSW_SERVICE_COMMONUTILS_H
#define BSW_SERVICE_COMMONUTILS_H

#include <string>


namespace Utils {

// 消息结构体
typedef struct {
    char command[100];      // 命令或问题
    uint8_t type;           // 消息类型: 0=用户输入, 1=API请求, 2=系统命令
    uint32_t timestamp;     // 时间戳
} input_message_t;

typedef struct {
    char question[200];     // 问题
    uint32_t request_id;    // 请求ID
    uint32_t timestamp;     // 时间戳
} api_request_message_t;

typedef struct {
    char answer[500];       // 回答
    uint32_t request_id;    // 请求ID
    uint32_t timestamp;     // 时间戳
    uint8_t status;         // 状态: 0=成功, 1=失败, 2=处理中
} response_message_t;


// 提供跨模块使用的工具函数
bool equalsIgnoreCase(const std::string& a, const std::string& b);
std::string trim(const std::string& str);
bool startsWith(const std::string& str, const std::string& prefix);
int indexOf(const std::string& str, const std::string& substr);
int lastIndexOf(const std::string& str, char ch);

} // namespace Utils

#endif