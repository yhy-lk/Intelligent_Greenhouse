/**
 * @file lvgl_app.cpp
 * @brief LVGL application layer implementation.
 */

#include "lvgl_app.h"

#include "app_config.h"
#include "custom.h"
#include "display_hal.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "gui_manager.h"

namespace App {
namespace LVGL {

static const char* TAG = "APP_LVGL";

static void lvgl_event_handler_task(void* pvParameters) {
    (void)pvParameters;
    ESP_LOGI(TAG, "LVGL event handler task started.");

    while (true) {
        // The queue-driven logic was removed. Without a blocking wait or an
        // explicit delay, this task spins forever on CPU0 and starves IDLE0.
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

static void lvgl_update_task(void* pvParameters) {
    (void)pvParameters;
    ESP_LOGI(TAG, "LVGL update task started.");

    while (true) {
        custom_update_screen_overview();
        hal_display_routine();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

bool init() {
    ESP_LOGI(TAG, "Initializing LVGL application layer...");

    hal_display_init();
    gui_init();

    BaseType_t ret = xTaskCreate(
        lvgl_event_handler_task,
        "ui_event_task",
        TASK_STACK_SIZE_SMALL,
        nullptr,
        TASK_PRIORITY_LOW,
        nullptr
    );

    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create UI event task.");
        return false;
    }

    ret = xTaskCreate(
        lvgl_update_task,
        "ui_update_task",
        TASK_STACK_SIZE_MEDIUM,
        nullptr,
        TASK_PRIORITY_NORMAL,
        nullptr
    );

    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create UI update task.");
        return false;
    }

    ESP_LOGI(TAG, "LVGL application layer initialized.");
    return true;
}

} // namespace LVGL
} // namespace App
