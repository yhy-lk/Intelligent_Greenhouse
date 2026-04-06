#include "os_service.h"
#include "global_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
// #include "board_config.h"
#include <Arduino.h>

namespace OsService {

void safe_delay_ms(uint32_t ms) {
#ifdef USE_FREERTOS
    vTaskDelay(pdMS_TO_TICKS(ms));
#elif
    delay(ms);
#endif
}

// 不能在中断服务函数中调用
void safe_printf(const char* format, ...) {
    // 1. 初始化互斥锁（建议放在 Setup 中，但这里为了保持函数独立性做了个空指针检查）
    static SemaphoreHandle_t print_mutex = NULL;
    if (print_mutex == NULL) {
        print_mutex = xSemaphoreCreateMutex();
        // 如果创建失败，直接返回，避免 Crash
        if (print_mutex == NULL) return; 
    }

    // 2. 尝试获取锁
    if (xSemaphoreTake(print_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        
        // 定义一个足够大的缓冲区
        // 注意：栈空间有限，不要定义过大，通常 256 或 512 字节足矣
        char loc_buf[512]; 

        va_list args;
        va_start(args, format);
        
        // 3. 核心修改：使用 vsnprintf 处理 va_list
        // vsnprintf 会将格式化后的字符串写入 loc_buf，并防止缓冲区溢出
        vsnprintf(loc_buf, sizeof(loc_buf), format, args);
        
        va_end(args);

        // 4. 打印最终的字符串
        Serial.print(loc_buf);
        
        // 5. 释放锁
        xSemaphoreGive(print_mutex);
    } else {
        // 可选：获取锁超时处理
        Serial.println("Print lock timeout");
    }
}

}