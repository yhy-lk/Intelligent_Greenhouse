/*
 * Copyright 2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

/*********************
 * INCLUDES
 *********************/
#include <stdio.h>
#include <math.h>
#include "lvgl.h"
#include "custom.h"
#include "sensor_state.h"
#include "can_network_service.h"
#include "esp_log.h"       // 引入 ESP-IDF 的日志库

// 引入全局 UI 结构体
extern lv_ui guider_ui;

/*********************
 * DEFINES
 *********************/
// 日志 TAG
static const char *TAG = "CUSTOM_UI";

// 默认主节点ID为1，可根据实际需求调整
#define OVERVIEW_NODE_ID 1

// 单位字符串
#define TEMP_UNIT       "°C"
#define HUMIDITY_UNIT   "%"
#define SOIL_UNIT       "%"
#define LIGHT_UNIT      "Lux"

/**********************
 * TYPEDEFS
 **********************/

/**********************
 * STATIC PROTOTYPES
 **********************/

/**********************
 * STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */
void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    ESP_LOGI(TAG, "Custom UI components initialized");
}

/**
 * @brief 更新总览界面的传感器数据
 */
void custom_update_screen_overview(void) 
{
    // ==========================================
    // 1. UI 状态与指针安全检查
    // ==========================================
    
    // 检查 UI 是否已初始化，以及对应的屏幕是否存在
    if (guider_ui.screen_overview == NULL) {
        ESP_LOGD(TAG, "UI not initialized or screen_overview is NULL. Aborting update.");
        return;
    }

    // 可选优化：如果当前显示的不是总览页面，可以选择跳过更新以节省 CPU 资源
    if (lv_disp_get_scr_act(NULL) != guider_ui.screen_overview) {
        ESP_LOGD(TAG, "Overview screen is not active, skipping UI update.");
        return;
    }

    // 检查需要操作的所有 Label 控件指针是否存在（防止 GUI Guider 重生成时改名导致的野指针崩溃）
    if (guider_ui.screen_overview_label_temperature_value == NULL ||
        guider_ui.screen_overview_label_humidity_value == NULL ||
        guider_ui.screen_overview_label_soil_station_value == NULL ||
        guider_ui.screen_overview_label_light_intensity_value == NULL) {
        ESP_LOGE(TAG, "One or more Label widgets are NULL. Check GUI Guider widget names.");
        return;
    }

    // ==========================================
    // 2. 获取底层传感器数据
    // ==========================================
    NodeData snapshot;
    if (!sensor_state_get_node_snapshot(OVERVIEW_NODE_ID, &snapshot)) {
        ESP_LOGW(TAG, "Failed to get snapshot for Node ID: %d. Showing placeholder.", OVERVIEW_NODE_ID);
        lv_label_set_text(guider_ui.screen_overview_label_temperature_value, "--");
        lv_label_set_text(guider_ui.screen_overview_label_humidity_value, "--");
        lv_label_set_text(guider_ui.screen_overview_label_soil_station_value, "--");
        lv_label_set_text(guider_ui.screen_overview_label_light_intensity_value, "--");
        return;
    }

    // ==========================================
    // 3. 数据解析与 UI 更新
    // ==========================================
    
    // 将 buf 大小从 16 增加到 32。防范负数、大浮点数或后续修改单位导致越界
    char buf[32]; 
    
    float temp     = snapshot.current_values[PARAM_IDX_TEMPERATURE];
    float humidity = snapshot.current_values[PARAM_IDX_HUMIDITY_AIR];
    float soil     = snapshot.current_values[PARAM_IDX_HUMIDITY_SOIL];
    float light    = snapshot.current_values[PARAM_IDX_LIGHT_INTENSITY];

    // --- 温度 ---
    if (temp != INVALID_VALUE) {
        snprintf(buf, sizeof(buf), "%.2f %s", temp / 100.0f, TEMP_UNIT);
        lv_label_set_text(guider_ui.screen_overview_label_temperature_value, buf);
    } else {
        ESP_LOGW(TAG, "Temperature value is invalid.");
        lv_label_set_text(guider_ui.screen_overview_label_temperature_value, "--");
    }

    // --- 空气湿度 ---
    if (humidity != INVALID_VALUE) {
        snprintf(buf, sizeof(buf), "%.2f %s", humidity / 100.0f, HUMIDITY_UNIT);
        lv_label_set_text(guider_ui.screen_overview_label_humidity_value, buf);
    } else {
        ESP_LOGW(TAG, "Air humidity value is invalid.");
        lv_label_set_text(guider_ui.screen_overview_label_humidity_value, "--");
    }

    // --- 土壤湿度 ---
    if (soil != INVALID_VALUE) {
        snprintf(buf, sizeof(buf), "%.2f %s", soil / 100.0f, SOIL_UNIT);
        lv_label_set_text(guider_ui.screen_overview_label_soil_station_value, buf);
    } else {
        ESP_LOGW(TAG, "Soil humidity value is invalid.");
        lv_label_set_text(guider_ui.screen_overview_label_soil_station_value, "--");
    }

    // --- 光照强度 ---
    if (light != INVALID_VALUE) {
        snprintf(buf, sizeof(buf), "%.0f %s", light, LIGHT_UNIT);
        lv_label_set_text(guider_ui.screen_overview_label_light_intensity_value, buf);
    } else {
        ESP_LOGW(TAG, "Light intensity value is invalid.");
        lv_label_set_text(guider_ui.screen_overview_label_light_intensity_value, "--");
    }

    ESP_LOGD(TAG, "Screen overview updated successfully.");
}

/**
 * @brief 处理遮阳篷/遮阳帘电机开关状态切换的 LVGL 事件
 * * @param e 指向 LVGL 事件对象的指针
 */
void custom_ui_handle_awning_sunshade_motor_switch_value_changed_event(lv_event_t * e)
{
    /* 1. 获取触发事件的开关对象及其当前状态 */
    lv_obj_t * sw_obj = lv_event_get_target(e);
    bool is_on = lv_obj_has_state(sw_obj, LV_STATE_CHECKED);
    float target_value = is_on ? 1.0f : 0.0f;

    /* 实际 MCU 硬件环境 */
    uint8_t target_node_id = 1;

    /* 假设 can_service_send_control 的声明你能通过某种方式（比如全局 extern 或者修改后的 custom.h）让 events_init.c 认识它 */
    bool req_sent = can_service_send_control(target_node_id, PARAM_IDX_SUNSHADE_MOTOR, target_value);

    if (!req_sent) {
        /* 使用 LVGL 的错误日志宏 */
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Awning control command!");
    } else {
        /* 使用 LVGL 的用户级日志宏 (相当于 INFO) */
        LV_LOG_USER("CUSTOM_UI: Awning control sent: %s", is_on ? "ON" : "OFF");
    }
}

/**
 * @brief 处理灌溉水泵开关状态切换的 LVGL 事件
 * * @param e 指向 LVGL 事件对象的指针
 */
void custom_ui_handle_water_pump_switch_value_changed_event(lv_event_t * e)
{
    /* 1. 获取触发事件的开关对象及其当前状态 */
    lv_obj_t * sw_obj = lv_event_get_target(e);
    bool is_on = lv_obj_has_state(sw_obj, LV_STATE_CHECKED);
    float target_value = is_on ? 1.0f : 0.0f;

    /* 实际 MCU 硬件环境 */
    uint8_t target_node_id = 1;

    bool req_sent = can_service_send_control(target_node_id, PARAM_IDX_WATER_PUMP, target_value);

    if (!req_sent) {
        /* 使用 LVGL 的错误日志宏 */
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Water pump control command!");
    } else {
        /* 使用 LVGL 的用户级日志宏 (相当于 INFO) */
        LV_LOG_USER("CUSTOM_UI: Water pump control sent: %s", is_on ? "ON" : "OFF");
    }
}

/**
 * @brief 处理加湿器开关状态切换的 LVGL 事件
 * * @param e 指向 LVGL 事件对象的指针
 */
void custom_ui_handle_humidifier_switch_value_changed_event(lv_event_t * e)
{
    /* 1. 获取触发事件的开关对象及其当前状态 */
    lv_obj_t * sw_obj = lv_event_get_target(e);
    bool is_on = lv_obj_has_state(sw_obj, LV_STATE_CHECKED);
    float target_value = is_on ? 1.0f : 0.0f;

    /* 实际 MCU 硬件环境 */
    uint8_t target_node_id = 1;

    bool req_sent = can_service_send_control(target_node_id, PARAM_IDX_HUMIDIFIER, target_value);

    if (!req_sent) {
        /* 使用 LVGL 的错误日志宏 */
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Humidifier control command!");
    } else {
        /* 使用 LVGL 的用户级日志宏 (相当于 INFO) */
        LV_LOG_USER("CUSTOM_UI: Humidifier control sent: %s", is_on ? "ON" : "OFF");
    }
}

/**
 * @brief 处理通风风扇转速滑块值变化的 LVGL 事件
 *
 * STM32 端存在双层控制逻辑：
 *   - PARAM_IDX_VENTILATION_FAN (0x13) = 主电源开关 (bool)
 *   - PARAM_IDX_FAN_SPEED       (0x35) = 目标转速设定 (u32, RPM)
 *
 * 当主开关为 OFF 时，STM32 会将目标转速强制覆写为 0。
 * 因此滑块变化时，必须先开启主开关，再发送目标转速。
 *
 * @param e 指向 LVGL 事件对象的指针
 */
void custom_ui_handle_ventilation_fan_speed_slider_value_changed_event(lv_event_t * e)
{
    /* 1. 从滑块控件中读取当前值 */
    int32_t slider_val = lv_slider_get_value(guider_ui.screen_manual_mode_slider_ventilation_fan_speed);

    /* 2. 将整数滑块值转换为浮点物理值 */
    float target_rpm = (float)slider_val;

    /* 实际 MCU 硬件环境 */
    uint8_t target_node_id = 1;

    /* 3. 先开启通风扇主电源开关 (PARAM_IDX_VENTILATION_FAN = 1.0f) */
    bool power_sent = can_service_send_control(target_node_id, PARAM_IDX_VENTILATION_FAN, 1.0f);
    if (!power_sent) {
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Ventilation fan power ON command!");
        return;
    }

    /* 4. 再发送目标转速 (PARAM_IDX_FAN_SPEED) */
    bool speed_sent = can_service_send_control(target_node_id, PARAM_IDX_FAN_SPEED, target_rpm);

    if (!speed_sent) {
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Ventilation fan speed control command!");
    } else {
        LV_LOG_USER("CUSTOM_UI: Ventilation fan power ON, speed sent: %ld rpm", (long)slider_val);
    }
}

/**
 * @brief 屏幕进入时通过 CAN 总线发送手动控制模式指令
 *
 * 当屏幕加载/导入时自动调用此函数，无条件地向目标节点发送手动模式 (0.0f) 指令，
 * 将系统切换为手动控制模式。
 */
void custom_ui_send_manual_control_mode_command_on_screen_enter(void)
{
    /* 实际 MCU 硬件环境 */
    uint8_t target_node_id = 1;

    /* 手动模式对应 PARAM_IDX_CONTROL_MODE = 0 */
    bool req_sent = can_service_send_control(target_node_id, PARAM_IDX_CONTROL_MODE, 0.0f);

    if (!req_sent) {
        LV_LOG_ERROR("CUSTOM_UI: Failed to send manual control mode command on screen enter!");
    } else {
        LV_LOG_USER("CUSTOM_UI: Manual control mode command sent on screen enter.");
    }
}

/**
 * @brief 处理补光灯亮度滑块值变化的 LVGL 事件
 *
 * 与通风风扇滑块类似的双层控制逻辑：
 *   - PARAM_IDX_LIGHT_MAIN_POWER (0x11) = 补光灯主电源开关 (bool)
 *   - PARAM_IDX_LIGHT_PWM_DUTY   (0x33) = PWM 占空比/亮度设定 (u32)
 *
 * 当主开关为 OFF 时，STM32 端可能忽略 PWM 值。
 * 因此滑块变化时，必须先开启主电源，再发送 PWM 占空比。
 *
 * @param e 指向 LVGL 事件对象的指针
 */
void custom_ui_handle_light_brightness_slider_value_changed_event(lv_event_t * e)
{
    /* 1. 从滑块控件中读取当前值 */
    int32_t slider_val = lv_slider_get_value(guider_ui.screen_manual_mode_slider_brightness_value);

    /* 2. 将整数滑块值转换为浮点物理值 */
    float target_pwm_duty = (float)slider_val;

    /* 实际 MCU 硬件环境 */
    uint8_t target_node_id = 1;

    /* 3. 先开启补光灯主电源开关 (PARAM_IDX_LIGHT_MAIN_POWER = 1.0f) */
    bool power_sent = can_service_send_control(target_node_id, PARAM_IDX_LIGHT_MAIN_POWER, 1.0f);
    if (!power_sent) {
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Light main power ON command!");
        return;
    }

    /* 4. 再发送 PWM 占空比/亮度 (PARAM_IDX_LIGHT_PWM_DUTY) */
    bool duty_sent = can_service_send_control(target_node_id, PARAM_IDX_LIGHT_PWM_DUTY, target_pwm_duty);

    if (!duty_sent) {
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Light brightness (PWM duty) control command!");
    } else {
        LV_LOG_USER("CUSTOM_UI: Light main power ON, brightness (PWM duty) sent: %ld", (long)slider_val);
    }
}

/**
 * @brief 处理补光灯颜色滑块值变化的 LVGL 事件
 *
 * 前端 UI 颜色带为逆序排列（红-洋红-蓝-青-绿-黄-红），
 * 使用公式: Hue = 360 - 滑块值 将 UI 滑块值 (0-360) 映射到 HSV 色相。
 * 固定 S=100%, V=100%，将 HSV 转换为 RGB 后组装成 0x00RRGGBB 格式发送。
 *
 * CAN 协议:
 *   - PARAM_IDX_LIGHT_MAIN_POWER (0x10) = 补光灯主电源开关 (bool)
 *   - PARAM_IDX_LIGHT_COLOR_RGB  (0x50) = 照明颜色 (0x00RRGGBB)
 *
 * @param e 指向 LVGL 事件对象的指针
 */
void custom_ui_handle_light_color_slider_value_changed_event(lv_event_t * e)
{
    /* 1. 从滑块控件中读取当前值 (0-360) */
    int32_t slider_val = lv_slider_get_value(guider_ui.screen_manual_mode_slider_color_adjustment);

    /* 2. 逆序映射: Hue = 360 - slider_val */
    int32_t hue = 360 - slider_val;
    if (hue < 0)   hue = 0;
    if (hue > 360)  hue = 360;

    /* 3. HSV (H, S=100%, V=100%) -> RGB 转换 */
    /*    算法: H'=H/60, X=1-|H'mod2-1|, 根据 H'整数部分查表 */
    uint8_t r, g, b;

    float h_prime = (float)hue / 60.0f;
    float x = 1.0f - fabsf(fmodf(h_prime, 2.0f) - 1.0f);
    /* 将 0.0~1.0 映射到 0~255，四舍五入 */
    uint8_t c255 = 255;
    uint8_t x255 = (uint8_t)(x * 255.0f + 0.5f);

    switch ((int)h_prime % 6) {
        case 0: r = c255; g = x255; b = 0;    break;
        case 1: r = x255; g = c255; b = 0;    break;
        case 2: r = 0;    g = c255; b = x255;  break;
        case 3: r = 0;    g = x255; b = c255;  break;
        case 4: r = x255; g = 0;    b = c255;  break;
        case 5: r = c255; g = 0;    b = x255;  break;
        default: r = 0; g = 0; b = 0; break;
    }

    /* 4. 组装为 0x00RRGGBB 格式 */
    uint32_t rgb_color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;

    /* 实际 MCU 硬件环境 */
    uint8_t target_node_id = 1;

    /* 5. 先开启补光灯主电源开关 (PARAM_IDX_LIGHT_MAIN_POWER = 1.0f) */
    bool power_sent = can_service_send_control(target_node_id, PARAM_IDX_LIGHT_MAIN_POWER, 1.0f);
    if (!power_sent) {
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Light main power ON command!");
        return;
    }

    /* 6. 发送颜色 (PARAM_IDX_LIGHT_COLOR_RGB)，将 uint32_t 强转为 float 传递 */
    bool color_sent = can_service_send_control(target_node_id, PARAM_IDX_LIGHT_COLOR_RGB, (float)rgb_color);

    if (!color_sent) {
        LV_LOG_ERROR("CUSTOM_UI: Failed to send Light color RGB control command!");
    } else {
        LV_LOG_USER("CUSTOM_UI: Light color sent: Hue=%ld, RGB=0x%06lX (R=%d, G=%d, B=%d)",
                    (long)hue, (unsigned long)rgb_color, r, g, b);
    }
}
