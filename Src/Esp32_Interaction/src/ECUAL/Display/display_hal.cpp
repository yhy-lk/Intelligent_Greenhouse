// display_hal.cpp - 显示硬件抽象层实现

#include "global_config.h"   // 必须包含全局配置头文件
#include "display_hal.h"
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lvgl.h>

// ==========================================
// 1. 硬件配置 (通过全局宏填入)
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
      cfg.port = DISPLAY_BUS_PORT;              
      cfg.freq_write = DISPLAY_FREQUENCY_WRITE; 
      cfg.pin_wr = DISPLAY_PIN_WR;           
      cfg.pin_rd = DISPLAY_PIN_RD;           
      cfg.pin_rs = DISPLAY_PIN_RS;           
      cfg.pin_d0 = DISPLAY_PIN_D0; 
      cfg.pin_d1 = DISPLAY_PIN_D1; 
      cfg.pin_d2 = DISPLAY_PIN_D2; 
      cfg.pin_d3 = DISPLAY_PIN_D3;
      cfg.pin_d4 = DISPLAY_PIN_D4; 
      cfg.pin_d5 = DISPLAY_PIN_D5; 
      cfg.pin_d6 = DISPLAY_PIN_D6; 
      cfg.pin_d7 = DISPLAY_PIN_D7;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs   = DISPLAY_PIN_CS;         
      cfg.pin_rst  = DISPLAY_PIN_RST;        
      cfg.pin_busy = DISPLAY_PIN_BUSY;         
      cfg.panel_width      = DISPLAY_PANEL_WIDTH;
      cfg.panel_height     = DISPLAY_PANEL_HEIGHT;
      cfg.offset_x         = DISPLAY_OFFSET_X;
      cfg.offset_y         = DISPLAY_OFFSET_Y;
      cfg.offset_rotation  = DISPLAY_OFFSET_ROTATION;
      cfg.dummy_read_pixel = DISPLAY_DUMMY_READ_PIXEL;
      cfg.dummy_read_bits  = DISPLAY_DUMMY_READ_BITS;
      // 以下为驱动核心逻辑状态，通常不需要宏定义
      cfg.readable         = false; 
      cfg.invert           = false; 
      cfg.rgb_order        = false; 
      cfg.dlen_16bit       = false; 
      cfg.bus_shared       = false; 
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl      = DISPLAY_PIN_BL;      
      cfg.invert      = DISPLAY_LIGHT_INVERT;   
      cfg.freq        = DISPLAY_BRIGHTNESS_FREQ;   
      cfg.pwm_channel = DISPLAY_BRIGHTNESS_CHANNEL;       
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    {
      auto cfg = _touch_instance.config();
      cfg.x_min      = DISPLAY_TOUCH_X_MIN;
      cfg.x_max      = DISPLAY_TOUCH_X_MAX;
      cfg.y_min      = DISPLAY_TOUCH_Y_MIN;
      cfg.y_max      = DISPLAY_TOUCH_Y_MAX;
      cfg.bus_shared = false;    
      cfg.offset_rotation = DISPLAY_OFFSET_ROTATION;   
      cfg.spi_host = DISPLAY_TOUCH_SPI_HOST;  
      cfg.freq     = DISPLAY_TOUCH_SPI_FREQ;    
      cfg.pin_sclk = DISPLAY_TOUCH_SPI_SCLK;         
      cfg.pin_cs   = DISPLAY_TOUCH_SPI_CS;         
      cfg.pin_miso = DISPLAY_TOUCH_SPI_MISO;  
      cfg.pin_mosi = DISPLAY_TOUCH_SPI_MOSI;  
      cfg.pin_int  = DISPLAY_TOUCH_INT_PIN;  
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
    pinMode(DISPLAY_TOUCH_SPI_MISO, INPUT_PULLUP);
    pinMode(DISPLAY_TOUCH_INT_PIN, INPUT_PULLUP);

    // 2. 初始化屏幕
    tft.init();
    tft.setRotation(DISPLAY_ROTATION); 
    tft.setBrightness(DISPLAY_BRIGHTNESS_DEFAULT); 

    // 3. 注入校准矩阵
    uint16_t calData[8] = DISPLAY_TOUCH_CAL_DATA;
    tft.setTouchCalibrate(calData);

    // 4. 初始化 LVGL 核心
    lv_init();
    lv_tick_set_cb(my_tick_get_cb); // 注册时钟回调

    // 5. 注册显示设备 (LVGL 9.x API)
    lv_display_t * disp = lv_display_create(DISPLAY_PANEL_WIDTH, DISPLAY_PANEL_HEIGHT);
    // 开辟绘制缓冲区
    static uint8_t draw_buf[DISPLAY_DRAW_BUF_SIZE]; 
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