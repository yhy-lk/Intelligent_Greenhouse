#include <Arduino.h>
#include "display_hal.h"
#include "gui_manager.h"

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("System Booting...");

    // 1. 初始化硬件抽象层 (屏幕、触摸、校准、LVGL内核)
    hal_display_init();

    // 2. 启动 NXP GUI Guider 生成的用户界面业务层
    gui_init();
}

void loop() {
    // 交出 CPU 时间，让 LVGL 调度刷新
    hal_display_routine();
    delay(5); 
}