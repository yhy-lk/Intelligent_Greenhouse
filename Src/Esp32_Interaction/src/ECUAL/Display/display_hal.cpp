#include "display_hal.h"
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lvgl.h>

// ==========================================
// 1. 硬件配置 (我们之前跑通的 LGFX 完整类)
// ==========================================
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341   _panel_instance;
  lgfx::Bus_Parallel8   _bus_instance;
  lgfx::Light_PWM       _light_instance;
  lgfx::Touch_XPT2046   _touch_instance;

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.port = 0;              
      cfg.freq_write = 20000000; 
      cfg.pin_wr = 12;           
      cfg.pin_rd = -1;           
      cfg.pin_rs = 13;           
      cfg.pin_d0 = 4; cfg.pin_d1 = 5; cfg.pin_d2 = 6; cfg.pin_d3 = 7;
      cfg.pin_d4 = 8; cfg.pin_d5 = 9; cfg.pin_d6 = 10; cfg.pin_d7 = 11;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs   = 14;         
      cfg.pin_rst  = 15;         
      cfg.pin_busy = -1;         
      cfg.panel_width      = 240;
      cfg.panel_height     = 320;
      cfg.offset_x         = 0;
      cfg.offset_y         = 0;
      cfg.offset_rotation  = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable         = false; 
      cfg.invert           = false; 
      cfg.rgb_order        = false; 
      cfg.dlen_16bit       = false; 
      cfg.bus_shared       = false; 
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl      = 16;      
      cfg.invert      = false;   
      cfg.freq        = 44100;   
      cfg.pwm_channel = 7;       
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    {
      auto cfg = _touch_instance.config();
      cfg.x_min      = 0;
      cfg.x_max      = 239;
      cfg.y_min      = 0;
      cfg.y_max      = 319;
      cfg.bus_shared = false;    
      cfg.offset_rotation = 0;   
      cfg.spi_host = SPI2_HOST;  
      cfg.freq     = 1000000;    
      cfg.pin_sclk = 17;         
      cfg.pin_cs   = 21;         
      cfg.pin_miso = 18;  // 硬件修复
      cfg.pin_mosi = 38;  // 硬件修复
      cfg.pin_int  = 39;  // 硬件中断
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};

LGFX tft;

// ==========================================
// 2. LVGL 对接回调函数
// ==========================================

// LVGL 显示刷新回调
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t * px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.writePixels((uint16_t *)px_map, w * h, true);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

// LVGL 触摸读取回调
void my_touch_read(lv_indev_t * indev, lv_indev_data_t * data) {
    uint16_t touchX, touchY;
    // LovyanGFX 底层已经内置了低通滤波，可以直接使用
    bool touched = tft.getTouch(&touchX, &touchY);

    if (!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

// 提供给 LVGL 9 的时钟心跳包
uint32_t my_tick_get_cb(void) {
    return millis();
}

// ==========================================
// 3. 开放给外部的 HAL 初始化函数
// ==========================================
void hal_display_init(void) {
    // 1. 防御性上拉
    pinMode(18, INPUT_PULLUP);
    pinMode(39, INPUT_PULLUP);

    // 2. 初始化屏幕
    tft.init();
    tft.setRotation(0); 
    tft.setBrightness(128); 

    // 3. 注入你的专属校准矩阵
    uint16_t calData[8] = { 295, 3583, 241, 147, 3800, 3584, 3720, 141 };
    tft.setTouchCalibrate(calData);

    // 4. 初始化 LVGL 核心
    lv_init();
    lv_tick_set_cb(my_tick_get_cb); // 注册时钟回调

    // 5. 注册显示设备 (LVGL 9.x API)
    lv_display_t * disp = lv_display_create(240, 320);
    // 开辟绘制缓冲区 (推荐大小至少为屏幕像素的 1/10，RGB565每个像素占2字节)
    static uint8_t draw_buf[240 * 32 * 2]; 
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, my_disp_flush);

    // 6. 注册输入设备 (LVGL 9.x API)
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touch_read);
}

void hal_display_routine(void) {
    // 处理 LVGL 的所有内部任务、动画、刷新
    lv_timer_handler(); 
}