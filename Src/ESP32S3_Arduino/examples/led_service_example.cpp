/**
 * FastLED 标准测试模板 (ESP32-S3)
 * 硬件: WS2812B (10颗) 接 GPIO 2
 */

#include <Arduino.h>
#include <LedService.h> // 引入神库

LedService led_service;

void setup() {
    Serial.begin(115200);
    delay(1000); // 上电缓冲，防止开机瞬间电流冲击
    Serial.println("FastLED Start!");

    led_service.begin(); // 初始化 LED 灯带
    led_service.setMode(MODE_LOADING); // 设置为跑马灯模式
}

void loop() {

    led_service.update();
    delay(20); // 每 20ms 更新一次画面（50FPS）

}