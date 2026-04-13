#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   1. 核心显示设置
 ====================*/
/* 颜色深度：LovyanGFX 和 ILI9341 我们配置的是 RGB565 */
#define LV_COLOR_DEPTH 16

/*====================
   2. 内存管理器设置
 ====================*/
/* LVGL 9 支持直接使用标准 C 库的 malloc (ESP32 的堆栈管理器非常强大，直接交由它管理) */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN
#define LV_MEM_SIZE (64 * 1024U) /* 给 LVGL 分配 64KB 的专属内存池 */

/*====================
   3. 调试与性能监控
 ====================*/
/* 开启日志输出，这在移植阶段排错极其重要 */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF 1

/* 开启左下角的 CPU 和 内存 占用率监视器，非常有极客感，排查卡顿利器 */
#define LV_USE_SYSMON 1
#define LV_USE_PERF_MONITOR 1
#define LV_USE_MEM_MONITOR 1

/*====================
   4. 字体与主题
 ====================*/
/* 启用默认主题和基础字体 */
#define LV_USE_THEME_DEFAULT 1
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_DEFAULT &lv_font_montserrat_12

/*====================
   5. 架构与硬件加速
 ====================*/
/* ESP32 是 Xtensa 架构，严禁启用 ARM 的软件汇编加速 */
#define LV_USE_DRAW_SW_ASM  LV_DRAW_SW_ASM_NONE

/* 新增此处：6. 官方演示与压测模块 */
#define LV_USE_DEMO_BENCHMARK 1

#endif /*LV_CONF_H*/