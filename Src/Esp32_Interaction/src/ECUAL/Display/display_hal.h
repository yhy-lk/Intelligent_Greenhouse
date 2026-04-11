#ifndef DISPLAY_HAL_H
#define DISPLAY_HAL_H

#include <Arduino.h>

// 初始化屏幕、触摸以及 LVGL 核心
void hal_display_init(void);

// LVGL 的心跳与任务调度例程（需放入主循环）
void hal_display_routine(void);

#endif // DISPLAY_HAL_H