// 这是一个示例文件，展示如何使用上述类。
// 将此逻辑放在您的main.cpp或专用服务中。

#include <Arduino.h>
#include "i2s_hal.h"
#include "inmp441_driver.h"
#include "max98357a_driver.h"

// 实例化驱动程序
Inmp441Driver mic;
Max98357aDriver amp;

// 一帧音频的缓冲区
const int FRAME_SIZE = 256;
int16_t audio_buffer[FRAME_SIZE];

void setup() {
    Serial.begin(115200);

    // 1. 初始化HAL（引擎）
    // 这必须在使用麦克风或放大器之前完成
    if (I2SHal::getInstance().begin()) {
        Serial.println("音频引擎已启动");
    } else {
        Serial.println("音频引擎启动失败！");
        while(1);
    }
}

void loop() {
    // 2. 从麦克风读取（消费者）
    int samples_read = mic.readFrame(audio_buffer, FRAME_SIZE);

    if (samples_read > 0) {
        // 可选：简单增益（数字音量）
        for (int i=0; i<samples_read; i++) {
            audio_buffer[i] = audio_buffer[i] * 2; // 音量加倍
        }

        // 3. 写入放大器（生产者）
        // 直接回环：输入什么，输出什么。
        amp.writeFrame(audio_buffer, samples_read);
    }
}