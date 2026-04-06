#![no_std]
#![no_main]

// Example originally designed for stm32f411ceu6 reading an A1454 hall effect sensor on I2C1
// DMA peripherals changed to compile for stm32f429zi, for the CI.

use cortex_m::prelude::_embedded_hal_blocking_i2c_Write;
use defmt::*;
use embassy_executor::Spawner;
use embassy_stm32::i2c::{Config, I2c};
use embassy_stm32::time::hz;
use embassy_stm32::{bind_interrupts, i2c, peripherals};
use {defmt_rtt as _, panic_probe as _};
// use embassy_stm32::dma::NoDma;

// SHT30 地址 (ADDR 接地)
const SHT30_ADDR: u8 = 0x44;

bind_interrupts!(struct Irqs {
    I2C2_EV => i2c::EventInterruptHandler<peripherals::I2C2>;
    I2C2_ER => i2c::ErrorInterruptHandler<peripherals::I2C2>;
});

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("Hello world!");
    let p = embassy_stm32::init(Default::default());

    let mut config = i2c::Config::default();

    config.frequency = embassy_stm32::time::Hertz::khz(400);
    config.scl_pullup = true;
    config.sda_pullup = true;

    let mut i2c = I2c::new_blocking(
        p.I2C2, 
        p.PB10, 
        p.PB11, 
        config
    );

    info!("1");
    i2c.blocking_write(SHT30_ADDR, &[0x30, 0xA2]);
    embassy_time::Timer::after_millis(2000).await;
    // match i2c.blocking_write(SHT30_ADDR, &[0x30, 0xA2]) {
    //     Ok(_) => info!("SHT30 Reset Command Sent"),
    //     Err(e) => error!("SHT30 Reset Failed: {:?}", e),
    // }
    info!("2");

    loop {
        let cmd_measure = [0x24, 0x00];
        info!("3");
        match i2c.write(SHT30_ADDR, &cmd_measure) {
            Ok(()) => {
                info!("4");
                embassy_time::Timer::after_millis(20).await;
                // --- 步骤 3: 读取数据 ---
                // 准备一个 6 字节的 buffer
                let mut data = [0u8; 6];
                // 发起读取请求。
                // 注意：这里不用 write_read，因为我们是分两步走的（发命令 -> 等 -> 读）
                info!("5");
                match i2c.blocking_read(SHT30_ADDR, &mut data) {
                    Ok(_) => {

                        info!("6");
                        // --- 步骤 4: 数据解析 ---
                        let temp_raw = u16::from_be_bytes([data[0], data[1]]);
                        let hum_raw  = u16::from_be_bytes([data[3], data[4]]);

                        // 简单的转换公式
                        let temp_c = -45.0 + 175.0 * (temp_raw as f32 / 65535.0);
                        let hum_rh = 100.0 * (hum_raw as f32 / 65535.0);

                        info!("Temp: {} °C, Hum: {} %", temp_c, hum_rh);
                        info!("7");
                        // TODO: 这里可以把数据通过 Channel 发送给 Dispatcher
                    },
                    Err(e) => error!("SHT30 Read Failed: {:?}", e),
                }      
                info!("8");
            },
            Err(e) => error!("I2C Error during read: {:?}", e),
        }
        embassy_time::Timer::after_millis(1000).await;
    }
}
