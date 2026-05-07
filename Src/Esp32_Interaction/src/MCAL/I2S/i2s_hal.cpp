#include "i2s_hal.h"
#include "esp_log.h" // 引入ESP-IDF日志宏

// 定义当前文件的日志标签
static const char *TAG = "I2S_HAL";

// i2s_hal.cpp - ESP32S I2S硬件抽象层实现

I2SHal& I2SHal::getInstance() {
    static I2SHal instance;
    return instance;
}

bool I2SHal::begin() {
    if (is_initialized) {
        ESP_LOGI(TAG, "[MCAL] I2S HAL已初始化，跳过重复配置");
        return true;
    }

    // 1. 配置I2S驱动（标准传统配置）
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX), // 全双工
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE, // 兼容INMP441
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,  // 立体声（标准）
        .communication_format = I2S_COMM_FORMAT_STAND_I2S, // Philips标准（1位移位）
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = I2S_DMA_BUF_COUNT,
        .dma_buf_len = I2S_DMA_BUF_LEN,
        .use_apll = false,
        .tx_desc_auto_clear = true, // 自动清除TX缓冲区，避免无数据发送时产生噪音
        .fixed_mclk = 0
    };

    // 2. 配置引脚 (通过全局宏填入)
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_PIN_BCLK,   // 共享BCLK
        .ws_io_num = I2S_PIN_LRCK,    // 共享LRCK
        .data_out_num = I2S_PIN_DOUT, // MAX98357A DIN
        .data_in_num = I2S_PIN_DIN    // INMP441 SD
    };

    // 3. 安装驱动
    esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        // 驱动安装失败属于严重错误，使用 ESP_LOGE
        ESP_LOGE(TAG, "I2S驱动安装失败: %d", err);
        return false;
    }

    // 4. 设置引脚
    err = i2s_set_pin(I2S_PORT, &pin_config);
    if (err != ESP_OK) {
        // 引脚配置失败属于严重错误，使用 ESP_LOGE
        ESP_LOGE(TAG, "I2S引脚设置失败: %d", err);
        return false;
    }

    // 5. 清除DMA缓冲区，防止启动噪音
    i2s_zero_dma_buffer(I2S_PORT);
    
    is_initialized = true;

    // 动态打印初始化成功的参数，属于常规流程信息，使用 ESP_LOGI
    ESP_LOGI(TAG, "[MCAL] I2S HAL初始化完成（全双工 %dkHz）", I2S_SAMPLE_RATE / 1000);
    return true;
}

size_t I2SHal::readRaw(void* dest, size_t size) {
    size_t bytes_read = 0;
    // 阻塞读取（portMAX_DELAY）以与DMA同步
    i2s_read(I2S_PORT, dest, size, &bytes_read, portMAX_DELAY);
    return bytes_read;
}

size_t I2SHal::writeRaw(const void* src, size_t size) {
    size_t bytes_written = 0;
    // 阻塞写入
    i2s_write(I2S_PORT, src, size, &bytes_written, portMAX_DELAY);
    return bytes_written;
}