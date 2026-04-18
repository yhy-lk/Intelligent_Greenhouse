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



} // namespace LVGL
} // namespace App

#endif // LVGL_APP_H