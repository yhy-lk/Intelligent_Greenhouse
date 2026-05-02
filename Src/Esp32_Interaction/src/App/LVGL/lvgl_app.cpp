/**
 * @file lvgl_app.cpp
 * @brief LVGL 应用程序层实现
 */

#include "lvgl_app.h"
#include "esp_log.h"
#include "custom.h"

// 硬件与 GUI 依赖
#include "display_hal.h"
#include "gui_manager.h"
#include "app_config.h" // 假设这里定义了任务优先级和栈大小




namespace App {
namespace LVGL {

static const char* TAG = "APP_LVGL";

/**
 * @brief 后端业务处理任务
 * 处理来自 UI 的异步事件，避免在 LVGL 线程中执行耗时操作
 */
static void lvgl_event_handler_task(void* pvParameters) {
    ESP_LOGI(TAG, "LVGL 后端事件任务已启动");
    int received_value = 0;
    
    while (true) {
        // 阻塞式接收队列消息
        // ...此处原本通过 slider_queue 接收消息的逻辑已移除...
    }
}

/**
 * @brief LVGL 刷新任务 (替代原本在 loop 中的 hal_display_routine)
 * 将 UI 刷新独立为一个任务，可以更精确地控制频率
 */
static void lvgl_update_task(void* pvParameters) {
    ESP_LOGI(TAG, "LVGL 刷新任务已启动");
    while (true) {
        custom_update_screen_overview();
        hal_display_routine();
        vTaskDelay(pdMS_TO_TICKS(5)); // 对应原本 loop 中的 delay(5)
    }
}

bool init() {
    ESP_LOGI(TAG, "正在初始化 LVGL 应用层...");



    // 2. 初始化硬件与 GUI 管理器
    hal_display_init();
    gui_init();

    // 3. 创建后台业务任务
    BaseType_t ret;
    ret = xTaskCreate(
        lvgl_event_handler_task,
        "ui_event_task",
        TASK_STACK_SIZE_SMALL,
        nullptr,
        TASK_PRIORITY_LOW,      // 优先级较低，属于业务处理
        nullptr
    );

    if (ret != pdPASS) {
        ESP_LOGE(TAG, "创建 UI 事件任务失败！");
        return false;
    }

    // 4. 创建 UI 定时刷新任务 (重要：UI 刷新通常需要较高优先级以保证流畅度)
    // 这里如果只给4096字节会导致栈溢出
    ret = xTaskCreate(
        lvgl_update_task,
        "ui_update_task",
        TASK_STACK_SIZE_MEDIUM,
        nullptr,
        TASK_PRIORITY_NORMAL, // 略低于 CAN 任务，但高于普通逻辑
        nullptr
    );

    if (ret != pdPASS) {
        ESP_LOGE(TAG, "创建 UI 刷新任务失败！");
        return false;
    }

    ESP_LOGI(TAG, "LVGL 应用层初始化完成");
    return true;
}



} // namespace LVGL
} // namespace App