#include "inmp441_driver.h"

Inmp441Driver::Inmp441Driver() : i2s(I2SHal::getInstance()) {
    // HAL由服务层或主函数初始化
}

int Inmp441Driver::readFrame(int16_t* buffer, int max_samples) {
    // 1. 我们需要 max_samples 个单声道数据。
    // 因为硬件是立体声，所以我们需要从 DMA 读两倍的数据量 (左+右)
    size_t bytes_to_read = max_samples * 2 * sizeof(int32_t);
    
    // 防止溢出我们的内部 raw_buffer
    if (bytes_to_read > RAW_BUFFER_SAMPLES * sizeof(int32_t)) {
        bytes_to_read = RAW_BUFFER_SAMPLES * sizeof(int32_t);
    }

    size_t bytes_read = i2s.readRaw(raw_buffer, bytes_to_read);
    
    // total_words_read 是包含左和右的总数据个数
    int total_words_read = bytes_read / sizeof(int32_t);

    int mono_samples = 0;
    
    // 2. 核心修复：步长为 2！只取偶数索引（左声道），跳过奇数索引（右声道）
    for (int i = 0; i < total_words_read; i += 2) {
        int32_t left_sample = raw_buffer[i];       // 左声道：真实声音
        // int32_t right_sample = raw_buffer[i+1]; // 右声道：垃圾数据，不管它

        // 位移处理 (将 24bit 高位对齐数据转为 16bit)
        int16_t pcm = (int16_t)(left_sample >> 14); 
        
        buffer[mono_samples] = pcm;
        mono_samples++;
    }

    // 返回实际提取出的单声道样本数
    return mono_samples; 
}
