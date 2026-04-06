#pragma once
// // ========================================
// // 用户配置文件
// // 所有配置均使用宏定义，方便用户修改
// // ========================================

// // FreeRTOS配置 - 重新启用，但使用更保守的配置
// #define USE_FREERTOS 1  // 1:启用FreeRTOS, 0:禁用FreeRTOS

// // 任务配置（当USE_FREERTOS=1时生效）
// #define TASK_STACK_SIZE_SMALL (4096)      // 小任务栈大小（字节）
// #define TASK_STACK_SIZE_MEDIUM (8192)     // 中等任务栈大小（字节）
// #define TASK_STACK_SIZE_LARGE (16384)      // 大任务栈大小（字节）
// #define TASK_PRIORITY_LOW 1             // 低优先级
// #define TASK_PRIORITY_NORMAL 2          // 正常优先级
// #define TASK_PRIORITY_HIGH 3            // 高优先级
// #define TASK_PRIORITY_CRITICAL 4        // 关键优先级

// // 队列配置 - 减小队列长度以节省内存
// #define QUEUE_LENGTH_INPUT 5            // 输入队列长度 - 从10减小
// #define QUEUE_LENGTH_API_REQUEST 3      // API请求队列长度 - 从5减小
// #define QUEUE_LENGTH_RESPONSE 3         // 响应队列长度 - 从5减小

// // Wi-Fi配置
// #define WIFI_SSID "423-2.4G"
// #define WIFI_PASSWORD "12345678"

// // DeepSeek API配置
// #define DEEPSEEK_API_KEY "sk-dac522d4a37942d5ae25379f9adad6d6"
// #define DEEPSEEK_API_URL "https://api.deepseek.com/v1/chat/completions"
// #define DEEPSEEK_MODEL "deepseek-chat"

// // DeepSeek API请求参数配置
// #define SYSTEM_ROLE_CONTENT "You are a helpful assistant. Please provide concise answers in English."
// #define MAX_TOKENS 200
// #define TEMPERATURE 0.7

// // 系统配置
// #define SERIAL_BAUD_RATE 9600
// #define WIFI_TIMEOUT_MS 10000
// #define API_TIMEOUT_MS 15000

// // 回答处理配置
// #define MAX_ANSWER_LENGTH 200
// #define ENABLE_ANSWER_SUMMARY false

// // 调试配置
// #define DEBUG_MODE true


