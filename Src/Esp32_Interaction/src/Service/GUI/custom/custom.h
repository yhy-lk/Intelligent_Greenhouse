/* custom.h */
#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

// ==========================================
// 针对 MCU 硬件环境包含所需的底层头文件
// ==========================================
#if !defined(_WIN32) && !defined(WIN32)
    #include "can_protocol.h"  // 包含 can_service_send_control 和 CanParamIndex
    #include "esp_log.h"       // 包含 ESP_LOGx 宏
#endif

// 你之前的自定义函数声明...
void custom_init(lv_ui *ui);
void custom_update_screen_overview(void);

#ifdef __cplusplus
}
#endif
#endif /* __CUSTOM_H_ */