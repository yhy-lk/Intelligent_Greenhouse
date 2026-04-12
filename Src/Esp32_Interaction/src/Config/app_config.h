#pragma once

#define TASK_STACK_SIZE_SMALL (4096)      // 小任务栈大小（字节）
#define TASK_STACK_SIZE_MEDIUM (8192)     // 中等任务栈大小（字节）
#define TASK_STACK_SIZE_LARGE (16384)      // 大任务栈大小（字节）

#define TASK_PRIORITY_LOW 1             // 低优先级
#define TASK_PRIORITY_NORMAL 2          // 正常优先级
#define TASK_PRIORITY_HIGH 3            // 高优先级
#define TASK_PRIORITY_CRITICAL 4        // 关键优先级

// 系统配置
#define WIFI_TIMEOUT_MS      10000
#define API_TIMEOUT_MS       15000


// DeepSeek API请求参数配置
#define SYSTEM_ROLE_CONTENT  "You are a helpful assistant. Please provide concise answers in English."
#define MAX_TOKENS            200
#define TEMPERATURE           0.7

