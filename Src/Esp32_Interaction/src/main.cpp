#include <Arduino.h>

// 引入 LovyanGFX 库
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// ==========================================
// 屏幕与触摸的引脚及硬件配置类
// ==========================================
class LGFX : public lgfx::LGFX_Device {
  // 定义硬件实例：ILI9341 面板，8位并口，PWM背光，XPT2046 触摸
  // (如果你后续发现颜色不对，可能是 ST7789，把 ILI9341 换成 ST7789 即可)
  lgfx::Panel_ILI9341   _panel_instance;
  lgfx::Bus_Parallel8   _bus_instance;
  lgfx::Light_PWM       _light_instance;
  lgfx::Touch_XPT2046   _touch_instance;

public:
  LGFX(void) {
    // 1. 配置 8 位并口总线 (8080 8-bit)
    {
      auto cfg = _bus_instance.config();
      cfg.port = 0;              // 使用 ESP32 的 I2S/LCD 外设端口
      cfg.freq_write = 20000000; // 写入频率 20MHz (如果花屏可以降低到 16000000)
      cfg.pin_wr = 12;           // WR 
      cfg.pin_rd = -1;           // RD 接了 3.3V，不使用引脚控制
      cfg.pin_rs = 13;           // RS/DC
      // 8位数据线
      cfg.pin_d0 = 4;
      cfg.pin_d1 = 5;
      cfg.pin_d2 = 6;
      cfg.pin_d3 = 7;
      cfg.pin_d4 = 8;
      cfg.pin_d5 = 9;
      cfg.pin_d6 = 10;
      cfg.pin_d7 = 11;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    // 2. 配置屏幕面板
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs   = 14;         // CS
      cfg.pin_rst  = 15;         // RST
      cfg.pin_busy = -1;         // 不使用 Busy 引脚
      cfg.panel_width      = 240;
      cfg.panel_height     = 320;
      cfg.offset_x         = 0;
      cfg.offset_y         = 0;
      cfg.offset_rotation  = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable         = false; // 由于没接 RD 引脚，设为不可读
      cfg.invert           = false; // 如果显示颜色反了 (比如黑变白)，将此设为 true
      cfg.rgb_order        = false; // 如果红蓝反了，将此设为 true
      cfg.dlen_16bit       = false; // 使用的是 8 位并口
      cfg.bus_shared       = false; // 并口未与触摸共享总线
      _panel_instance.config(cfg);
    }

    // 3. 配置 PWM 背光
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl      = 16;      // BL
      cfg.invert      = false;   // 高电平点亮
      cfg.freq        = 44100;   // PWM 频率
      cfg.pwm_channel = 7;       // ESP32 PWM 通道 (避开常用的 0-2 通道)
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    // 4. 配置 SPI 触摸屏 (XPT2046)
    {
      auto cfg = _touch_instance.config();
      cfg.x_min      = 0;
      cfg.x_max      = 239;
      cfg.y_min      = 0;
      cfg.y_max      = 319;
      cfg.pin_int    = -1;       // T_IRQ (中断引脚)
      cfg.bus_shared = false;    // 触摸总线是独立的 SPI，不共享
      cfg.offset_rotation = 0;   // 如果触摸方向不对，修改这个旋转值(0-3)

      cfg.spi_host = SPI2_HOST;  // ESP32-S3 推荐使用 SPI2_HOST (FSPI)
      cfg.freq     = 500000;    // 触摸 SPI 频率 (XPT2046 不需要太高，1MHz-2MHz 即可)
      cfg.pin_sclk = 17;         // T_CLK
      cfg.pin_mosi = 18;         // T_MOSI
      cfg.pin_miso = 38;         // T_MISO
      cfg.pin_cs   = 21;         // T_CS
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    // 挂载面板
    setPanel(&_panel_instance);
  }
};

// ==========================================
// 全局实例化屏幕对象
// ==========================================
LGFX tft;

void setup() {
  Serial.begin(9600);
  Serial.println("System Starting...");

  // 初始化屏幕和触摸
  tft.init();
  
  // 设置屏幕方向 (0=竖屏, 1=横屏, 2=反向竖屏, 3=反向横屏)
  tft.setRotation(0); 
  
  // 设置背光亮度 (0-255)
  tft.setBrightness(128); 

  // 简单的 UI 测试
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("LovyanGFX Ready!");
  tft.println("Touch the screen...");
}

void loop() {
  uint16_t x_raw, y_raw;
  
  // getTouchRaw 不做任何过滤，直接返回 SPI 读到的原始数值 (通常是 0-4095)
  if (tft.getTouchRaw(&x_raw, &y_raw)) {
    Serial.printf("RAW X: %d, RAW Y: %d\n", x_raw, y_raw);
  } else {
    // 为了防止刷屏太快，可以加个延时，或者在按压时才打印
    Serial.println("No Raw Data...");
  }
  delay(200);
}