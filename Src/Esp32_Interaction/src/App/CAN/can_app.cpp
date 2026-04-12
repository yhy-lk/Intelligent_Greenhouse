/**
 * @file can_app.cpp
 * @brief CAN 应用程序层实现
 */

#include "can_app.h"
#include "esp_log.h"

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 引入纯 C 实现的 CAN 网络服务
// 因为当前是 .cpp 文件，所以依赖的 C 头文件内部必须有 extern "C"（我们之前已经加了）
#include "can_network_service.h"

#include "app_config.h"
#include "sensor_state.h"
#include "global_config.h"

namespace App {
namespace CAN {

static const char* TAG = "APP_CAN";

// 任务句柄，用于可能的生命周期管理
static TaskHandle_t s_can_task_handle = nullptr;

/**
 * @brief CAN 核心轮询任务
 * * @param arg 传递给任务的参数（当前未使用）
 */
static void can_rx_task(void* arg) {
    ESP_LOGI(TAG, "CAN 轮询任务已启动, 运行在 Core %d", xPortGetCoreID());

    // 任务主循环
    while (true) {
        // 调用底层的轮询引擎，内部会自动解析并写入 sensor_state 单例
        can_service_poll();

        // 适当让出 CPU。10ms 的轮询周期对温室系统（主要监控温湿度等慢速变量）来说绰绰有余
        // 同时能确保不饿死低优先级任务 (如空闲任务和看门狗)
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // 理论上不会执行到这里，FreeRTOS 任务不能直接 return，必须显式删除
    vTaskDelete(nullptr);
}

bool init() {
    ESP_LOGI(TAG, "正在初始化 CAN 应用层...");

    // 1. 检查是否已经初始化过
    if (s_can_task_handle != nullptr) {
        ESP_LOGW(TAG, "CAN 任务已存在，避免重复初始化");
        return true;
    }

    sensor_state_init();
    // 2. 配置并初始化底层 CAN 服务
    CanConfig config = CAN_CONFIG_DEFAULT((gpio_num_t)CAN_TX_PIN, (gpio_num_t)CAN_RX_PIN, CAN_BAUD_RATE);
    
    // 默认宏中队列大小是 20，如果是通信密集的网络，可以在这里单独调大
    config.queue_size = 30; 

    if (!can_service_init(&config)) {
        ESP_LOGE(TAG, "CAN 底层服务初始化失败！");
        return false;
    }

    // 3. 创建 FreeRTOS 任务
    // 参数说明：任务函数, 任务名称, 栈大小(字节/字), 参数, 优先级, 任务句柄
    // 栈大小给 4096 (1024 words) 足够处理 CAN 解析和 ESP_LOG
    BaseType_t ret = xTaskCreate(
        can_rx_task,        // 任务函数指针
        "can_rx_task",      // 任务名称（调试用）
        TASK_STACK_SIZE_SMALL,               // 任务栈大小
        nullptr,            // 传递参数
        TASK_PRIORITY_CRITICAL,                  // 任务优先级 (建议比 UI 任务略高)
        &s_can_task_handle  // 任务句柄
    );

    if (ret != pdPASS) {
        ESP_LOGE(TAG, "创建 CAN 轮询任务失败！");
        return false;
    }

    ESP_LOGI(TAG, "CAN 应用层初始化完成");
    return true;
}

} // namespace CAN
} // namespace App