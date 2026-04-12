#pragma once

// i2s_hal.h - ESP32S I2S硬件抽象层

#include <Arduino.h>
#include <driver/i2s.h>
#include "global_config.h" // 统一使用全局配置文件

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
    static const int SAMPLE_RATE = I2S_SAMPLE_RATE;       // 采样率
    static const int DMA_BUF_LEN = I2S_DMA_BUF_LEN;       // DMA缓冲区长度
    static const int DMA_BUF_COUNT = I2S_DMA_BUF_COUNT;   // DMA缓冲区数量

private:
    I2SHal() {} // 私有构造函数
    const i2s_port_t I2S_PORT = I2S_PORT_NUM;             // 使用宏定义的I2S端口
};