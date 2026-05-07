/**
 * @file voice_assistant_bridge.h
 * @brief 语音助手服务的 C 语言桥接接口
 *
 * 提供 extern "C" 链接的函数，使 C 代码（如 custom.c）能够
 * 启动和停止语音助手服务。
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * @brief 启动语音助手服务
 *
 * 初始化音频引擎、百度 API，并在 Core 1 上创建后台任务。
 * 如果服务已在运行，则跳过启动。
 *
 * @return true 启动成功，false 初始化失败
 */
bool voice_assistant_start(void);

/**
 * @brief 停止语音助手服务
 *
 * 安全地停止后台任务并重置状态机。
 */
void voice_assistant_stop(void);

#ifdef __cplusplus
}
#endif