// #![no_std]
use embassy_stm32::{i2c, mode, peripherals};
use embassy_sync::{blocking_mutex, blocking_mutex::raw::ThreadModeRawMutex, mutex, signal::Signal};
use ssd1306::Ssd1306Async;
// use embassy_stm32::timer::{simple_pwm};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::mutex::Mutex;

use static_cell::StaticCell;
use core::cell::RefCell;

// ================================ AsyncMasterI2c ================================
pub type AsyncMasterI2c = i2c::I2c<'static, mode::Async, i2c::Master>;
pub type AsyncMasterI2cDevice = embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice<
    'static,
    ThreadModeRawMutex,
    AsyncMasterI2c,
>;
pub type MutexAsyncMasterI2c = mutex::Mutex<ThreadModeRawMutex, AsyncMasterI2c>;
pub type StaticCellAsyncMasterI2cBus = StaticCell<MutexAsyncMasterI2c>;

// ================================ BlockingMasterI2c ================================
pub type BlockingMasterI2c = i2c::I2c<'static, mode::Blocking, i2c::Master>;
pub type BlockingMasterI2cDevice = embassy_embedded_hal::shared_bus::blocking::i2c::I2cDevice<
    'static,
    ThreadModeRawMutex,
    BlockingMasterI2c,
>;
pub type MutexBlockingMasterI2c = blocking_mutex::Mutex<ThreadModeRawMutex, RefCell<BlockingMasterI2c>>;
pub type StaticCellBlockingMasterI2cBus = StaticCell<MutexBlockingMasterI2c>;

// ================================ OLED Display ================================
pub type ConcreteDisplay = Ssd1306Async<
    ssd1306::prelude::I2CInterface<AsyncMasterI2cDevice>,
    ssd1306::size::DisplaySize128x64,
    ssd1306::mode::BufferedGraphicsModeAsync<ssd1306::size::DisplaySize128x64>,
>;

// ================================ LED Signal ================================
pub type LedSingal = Signal<ThreadModeRawMutex, u32>;

// ================================ WS2812 ================================
pub type Ws2812 = bsw::ws2812::Ws2812<'static, peripherals::TIM4, peripherals::DMA1_CH6>;
// pub type Ws2812Receiver = embassy_sync::channel::Receiver<'static, embassy_sync::blocking_mutex::raw::ThreadModeRawMutex, smart_leds::RGB8, 10>;
pub type Ws2812Sender = embassy_sync::channel::Sender<'static, embassy_sync::blocking_mutex::raw::ThreadModeRawMutex, smart_leds::RGB8, 10>;

use bsw::protocol::{CurrentValues, TargetValues, SystemState};

pub static GLOBAL_STATE: Mutex<CriticalSectionRawMutex, SystemState> = 
    Mutex::new(SystemState {
        current: CurrentValues {
            ..CurrentValues::default_impl() // Rust 默认宏无法应用于 const 上下文的 workaround，见下方
        },
        target: TargetValues {
            ..TargetValues::default_impl()
        },
    });

