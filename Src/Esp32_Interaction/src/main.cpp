#include <Arduino.h>
#include "display_hal.h"
#include "gui_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// 1. 定义一个全局队列句柄 (对应 custom.c 里的 extern)
QueueHandle_t slider_queue = NULL;

// 2. 定义后端专用的业务处理任务 (纯后台运算，完全不影响 UI 帧率)
void Backend_Print_Task(void *pvParameters) {
    int received_value = 0;
    
    while(1) {
        // 死等队列里的数据。portMAX_DELAY 意味着没数据时任务进入阻塞态，绝对不占 CPU
        if (xQueueReceive(slider_queue, &received_value, portMAX_DELAY) == pdTRUE) {
            // 这里就是真正的原子性操作边界！数据已经安全取到了后端！
            Serial.printf("[Backend Task] 滑块最新值: %d\n", received_value);
            
            // 你以后可以在这里调用你的 Service 层，比如：
            // can_network_send(received_value);
            // set_greenhouse_temperature(received_value);
        }
    }
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("System Booting...");

    // 创建一个深度为 10，每个元素为 int 大小的队列
    slider_queue = xQueueCreate(10, sizeof(int));

    // 创建一个独立的后台任务来处理数据 (分配 2048 字节栈，优先级设为 1)
    xTaskCreate(Backend_Print_Task, "BackendTask", 2048, NULL, 1, NULL);

    // 初始化硬件与 UI
    hal_display_init();
    gui_init();
}

void loop() {
    // 前端 UI 刷新仍然在主循环（也就是 FreeRTOS 的主任务）中无脑跑
    hal_display_routine();
    delay(5); 
}