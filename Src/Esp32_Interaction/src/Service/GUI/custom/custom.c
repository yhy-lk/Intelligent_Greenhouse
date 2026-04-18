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
#include "lvgl.h"
#include "custom.h"
#include "sensor_state.h"
#include "can_protocol.h"
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