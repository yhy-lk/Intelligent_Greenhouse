/**
 * @file lvgl_app.h
 * @brief LVGL 应用程序层 - 负责 UI 初始化与后台事件处理任务
 */

#ifndef LVGL_APP_H
#define LVGL_APP_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

namespace App {
namespace LVGL {

/**
 * @brief 初始化 LVGL 应用
 * 1. 初始化显示驱动与 GUI 引擎
 * 2. 创建 UI 事件处理队列
 * 3. 启动 UI 定时刷新任务与后台业务任务
 * @return true 初始化成功
 */
bool init();

/**
 * @brief 获取滑块事件队列句柄
 * 用于其他模块（如 GUI 回调）向后台发送数据
 */
QueueHandle_t get_slider_queue();

} // namespace LVGL
} // namespace App

#endif // LVGL_APP_H