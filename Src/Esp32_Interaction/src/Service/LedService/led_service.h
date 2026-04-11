#pragma once
#include <FastLED.h>

// 硬件配置
#define LED_PIN     2
#define NUM_LEDS    12
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

enum LedMode {
    MODE_OFF,
    MODE_BREATHING,
    MODE_LOADING,
    MODE_ALERT
};

class LedService {
public:
    void begin() {
        // 只负责硬件初始化，不创建任务
        FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
               .setCorrection(TypicalLEDStrip);
        FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
        FastLED.setBrightness(50);
    }

    // 设置模式（纯内存操作，极其快速）
    void setMode(LedMode mode) {
        currentMode = mode;
    }

    // 【核心】更新一帧画面
    // 这个函数由上层负责定时调用（例如每 20ms 调用一次）
    void update() {
        switch (currentMode) {
            case MODE_OFF:
                fill_solid(leds, NUM_LEDS, CRGB::Black);
                break;
            case MODE_BREATHING:
                // FastLED 内部基于 millis() 计算，所以只要调用够快，动画就是准的
                // 不需要我们传 dt 时间差
                {
                    uint8_t b = beatsin8(30, 20, 150);
                    fill_solid(leds, NUM_LEDS, CHSV(96, 255, b));
                }
                break;
            case MODE_LOADING:
                {
                    // 1. 拖尾效果：每次变暗一点点
                    fadeToBlackBy(leds, NUM_LEDS, 20);
                    // 2. 计算位置
                    uint16_t pos = (millis() / 100) % NUM_LEDS;
                    // 3. 赋值（不用 +=，防止刷新率波动时颜色溢出）
                    leds[pos] = CRGB::Blue;
                }
                break;
            case MODE_ALERT:
                {
                    // 简单的红色爆闪 (每250ms翻转)
                    if ((millis() / 250) & 1) fill_solid(leds, NUM_LEDS, CRGB::Red);
                    else fill_solid(leds, NUM_LEDS, CRGB::Black);
                }
                break;
        }
        // 推送数据给硬件
        FastLED.show();
    }

private:
    CRGB leds[NUM_LEDS];
    LedMode currentMode = MODE_OFF;
};