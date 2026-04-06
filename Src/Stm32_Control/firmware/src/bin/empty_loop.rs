#![no_std]
#![no_main]

use defmt::info;
// use defmt::info;
use embassy_executor::Spawner;

use {defmt_rtt as _, panic_probe as _};

mod domains;
mod shared;
use domains::config;

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    // 1. 初始化 MCU (BSW 内部细节)
    let _p = embassy_stm32::init(config::SystemConfig::system_clock_config());
    let mut num = 0;
    loop {
        num += 1;
        info!("num = {}", num);
        embassy_time::Timer::after_millis(10).await;
    }
}



