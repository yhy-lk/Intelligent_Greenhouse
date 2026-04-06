#include "max98357a_driver.h"

Max98357aDriver::Max98357aDriver() : i2s(I2SHal::getInstance()) {
}

void Max98357aDriver::writeFrame(const int16_t* buffer, int samples) {
    if (samples > MAX_SAMPLES) samples = MAX_SAMPLES;

    // 1. 格式转换
    // 我们有16位单声道。I2S硬件期望32位立体声（通常）。
    // 我们需要将单声道复制到左右声道，并向上移位。
    
    for (int i = 0; i < samples; i++) {
        int16_t sample = buffer[i];
        
        // 将16位转换回32位（左对齐）
        // 如果我们向下移位14位来读取，我们向上移位16位来写入满量程
        // 或者直接 << 16 将16位放入32位的MSB。
        int32_t i2s_sample = ((int32_t)sample) << 16; 

        // 立体声交错（左 + 右）
        // 由于是单声道数据，我们将相同的数据发送到两个扬声器（如果有两个）
        tx_buffer[i*2]     = i2s_sample; // 左声道
        tx_buffer[i*2 + 1] = i2s_sample; // 右声道
    }

    // 2. 发送到硬件
    // 总字节数 = 样本数 * 2（声道） * 4（每个样本的字节数）
    i2s.writeRaw(tx_buffer, samples * 2 * sizeof(int32_t));
}