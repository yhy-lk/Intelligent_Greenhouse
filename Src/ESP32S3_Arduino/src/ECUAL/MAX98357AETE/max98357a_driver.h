#pragma once

#include "MCAL/i2s_hal.h"

class Max98357aDriver {
public:
    Max98357aDriver();

    // 发送16位PCM音频到放大器
    void writeFrame(const int16_t* buffer, int samples);

private:
    I2SHal& i2s;
    
    // 临时缓冲区，用于将16位单声道转换为32位立体声（I2S标准要求）
    static const int MAX_SAMPLES = 256;
    int32_t tx_buffer[MAX_SAMPLES * 2]; // 立体声需要2倍空间
};