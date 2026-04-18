#pragma once 

// global_config.h - 全局配置文件，包含所有模块的公共配置项

#define USE_FREERTOS 1 

// =================== CAN 通信配置 ===================
#define CAN_TX_PIN                  47
#define CAN_RX_PIN                  48
#define CAN_BAUD_RATE               1000000
#define CAN_MODE                    TWAI_MODE_NORMAL

// =================== DISPLAY 配置 ===================

// --- 1. 基础屏幕参数 ---
#define DISPLAY_PANEL_WIDTH         240
#define DISPLAY_PANEL_HEIGHT        320
#define DISPLAY_ROTATION            0       // 默认屏幕旋转角度 (0-3)
#define DISPLAY_BRIGHTNESS_DEFAULT  255     // 默认背光亮度 (0-255)

// --- 2. 并口(8080)总线配置 ---
#define DISPLAY_BUS_PORT            0
#define DISPLAY_FREQUENCY_WRITE     20000000
#define DISPLAY_PIN_WR              12
#define DISPLAY_PIN_RD              -1
#define DISPLAY_PIN_RS              13
#define DISPLAY_PIN_D0              4
#define DISPLAY_PIN_D1              5
#define DISPLAY_PIN_D2              6
#define DISPLAY_PIN_D3              7
#define DISPLAY_PIN_D4              8
#define DISPLAY_PIN_D5              9
#define DISPLAY_PIN_D6              10
#define DISPLAY_PIN_D7              11

// --- 3. 面板控制引脚 & 偏移参数 ---
#define DISPLAY_PIN_CS              14
#define DISPLAY_PIN_RST             15
#define DISPLAY_PIN_BUSY            -1
#define DISPLAY_OFFSET_X            0
#define DISPLAY_OFFSET_Y            0
#define DISPLAY_OFFSET_ROTATION     0
#define DISPLAY_DUMMY_READ_PIXEL    8
#define DISPLAY_DUMMY_READ_BITS     1

// --- 4. 背光(PWM)配置 ---
#define DISPLAY_PIN_BL              16
#define DISPLAY_BRIGHTNESS_FREQ     44100
#define DISPLAY_BRIGHTNESS_CHANNEL  7
#define DISPLAY_LIGHT_INVERT        false   // 背光逻辑是否反转

// --- 5. 触摸屏(SPI)配置 ---
#define DISPLAY_TOUCH_SPI_HOST      SPI2_HOST
#define DISPLAY_TOUCH_SPI_FREQ      1000000
#define DISPLAY_TOUCH_SPI_SCLK      17      // 原命名CLOCK，改为SCLK更贴合SPI规范
#define DISPLAY_TOUCH_SPI_MOSI      38
#define DISPLAY_TOUCH_SPI_MISO      18      // 【已修正】原为17，根据代码逻辑应为18
#define DISPLAY_TOUCH_SPI_CS        21
#define DISPLAY_TOUCH_INT_PIN       39
#define DISPLAY_TOUCH_X_MIN         0
#define DISPLAY_TOUCH_X_MAX         (DISPLAY_PANEL_WIDTH - 1)
#define DISPLAY_TOUCH_Y_MIN         0
#define DISPLAY_TOUCH_Y_MAX         (DISPLAY_PANEL_HEIGHT - 1)

// 触摸屏校准数据 (根据具体屏幕实际校准值修改)
#define DISPLAY_TOUCH_CAL_DATA      { 295, 3583, 241, 147, 3800, 3584, 3720, 141 }

// --- 6. LVGL 缓冲配置 ---
// 推荐至少为屏幕像素的1/10，RGB565每个像素占2字节
#define DISPLAY_DRAW_BUF_SIZE       (DISPLAY_PANEL_WIDTH * 32 * 2)


// =================== I2S 音频配置 ===================
// 基础参数
#define I2S_PORT_NUM                I2S_NUM_0
#define I2S_SAMPLE_RATE             16000
#define I2S_BITS_PER_SAMPLE         I2S_BITS_PER_SAMPLE_32BIT

// DMA 缓存配置
#define I2S_DMA_BUF_COUNT           4
#define I2S_DMA_BUF_LEN             1024

// I2S 引脚配置
#define I2S_PIN_BCLK                40  // 共享 BCLK (位时钟)
#define I2S_PIN_LRCK                41  // 共享 LRCK/WS (左右声道时钟)
#define I2S_PIN_DOUT                42  // MAX98357A DIN (I2S 数据输出)
#define I2S_PIN_DIN                 1   // INMP441 SD (I2S 数据输入)



