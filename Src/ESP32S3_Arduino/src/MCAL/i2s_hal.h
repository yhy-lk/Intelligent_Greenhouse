#pragma once

#include <Arduino.h>
#include <driver/i2s.h>
#include "Config/board_config.h" // 假设引脚定义在这里

// ESP32 (IDF 4.4) 的单例I2S管理器
class I2SHal {
public:
    static I2SHal& getInstance();

    // 初始化I2S全双工模式（主模式，TX + RX）
    bool begin();

    // 原始读取（从DMA到缓冲区）
    // 返回读取的字节数
    size_t readRaw(void* dest, size_t size);

    // 原始写入（从缓冲区到DMA）
    // 返回写入的字节数
    size_t writeRaw(const void* src, size_t size);

    // 获取配置（供上层了解缓冲区大小等）
    static const int SAMPLE_RATE = 16000;     // 采样率：16kHz
    static const int DMA_BUF_LEN = 1024;      // DMA缓冲区长度
    static const int DMA_BUF_COUNT = 4;       // DMA缓冲区数量

private:
    I2SHal() {} // 私有构造函数
    const i2s_port_t I2S_PORT = I2S_NUM_0;    // 使用I2S端口0
};