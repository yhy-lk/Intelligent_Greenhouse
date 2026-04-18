#include <Arduino.h>
#include "esp_log.h"

// 引入应用层
#include "App/LVGL/lvgl_app.h"
#include "App/CAN/can_app.h"

static const char* TAG = "MAIN";

// 如果你的 custom.c 还需要直接访问该句柄，
// 可以在这里声明，或者在 lvgl_app.cpp 中通过外部链接导出


void setup() {
    // 虽然改用了 ESP_LOG，但在某些 ESP32 环境下仍需 Serial.begin 初始化串口底层
    Serial.begin(115200);
    delay(10000);
    
    ESP_LOGI(TAG, "System Booting...");

    // 1. 初始化 CAN 应用层 (使用之前定义的引脚和波特率)
    if (!App::CAN::init()) {
        ESP_LOGE(TAG, "CAN 模块启动失败");
    }
    delay(100); // 确保 CAN 模块稳定后再继续
    
    // 2. 初始化 LVGL 应用层 (包含 UI 任务和后台任务)
    if (!App::LVGL::init()) {
        ESP_LOGE(TAG, "LVGL 模块启动失败");
    }
    delay(100); // 确保 CAN 模块稳定后再继续



    ESP_LOGI(TAG, "所有模块初始化尝试完成");
}

void loop() {
    // 所有的逻辑都在 FreeRTOS 任务中了
    // loop 任务现在可以作为监控任务，或者干脆空转/删除
    vTaskDelay(pdMS_TO_TICKS(1000));
}