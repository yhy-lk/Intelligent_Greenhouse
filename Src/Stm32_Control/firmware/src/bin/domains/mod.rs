
use {defmt_rtt as _, panic_probe as _};
// use core::cell::RefCell;
// pub type ConcreteDisplay = Ssd1306Async<ssd1306::prelude::I2CInterface<ConcreteI2c>, ssd1306::size::DisplaySize128x64, ssd1306::mode::BufferedGraphicsModeAsync<ssd1306::size::DisplaySize128x64>>;

// pub static I2C1_MUTEX: MutexI2c = Mutex::new(None);

// 引入我们的层
pub mod board_led;
pub mod ui;
pub mod config;
pub mod ws2812;
pub mod dispatcher;
pub mod sht30;
pub mod soil_moisture;
pub mod mg995;
pub mod bh1750;
pub mod water_pump;
pub mod humidifier;
pub mod can_rx;
pub mod can_tx;
pub mod ventilation_fan;