#include "gui_manager.h"
// 引入 GUI Guider 生成的核心头文件
#include "gui_guider.h"
#include "events_init.h"

// GUI Guider 声明的全局 UI 结构体对象
lv_ui guider_ui;

void gui_init(void) {
    // 1. 初始化 GUI Guider 生成的所有界面
    setup_ui(&guider_ui);
    
    // 2. 初始化你在 GUI Guider 里绑定的所有事件
    events_init(&guider_ui);
}
