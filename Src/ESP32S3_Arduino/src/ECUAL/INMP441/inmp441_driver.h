#pragma once

#include "MCAL/i2s_hal.h"
#include <vector>

class Inmp441Driver {
public:
    Inmp441Driver();
    
    // 读取一帧音频，处理它，并填充提供的缓冲区
    // 返回实际读取的样本数
    int readFrame(int16_t* buffer, int max_samples);

private:
    I2SHal& i2s;
    
    // 用于存储从DMA读取的原始32位数据的临时缓冲区
    // 大小 = 1024字节（DMA长度）/ 4字节 = 256个int32样本
    static const int RAW_BUFFER_SAMPLES = 256; 
    int32_t raw_buffer[RAW_BUFFER_SAMPLES];
};