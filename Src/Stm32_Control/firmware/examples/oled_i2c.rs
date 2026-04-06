//! Example of I2C OLED display using DMA with Embassy on STM32F4.
//!
//! This example demonstrates how to use the I2C peripheral with DMA to drive
//! an SSD1306 OLED display without blocking the CPU.

#![no_std]
#![no_main]

use core::fmt::Write;

use defmt::{error, info};
use embassy_executor::Spawner;
use embassy_stm32::{
    Peri, bind_interrupts,
    gpio::{Level, Output, Speed},
    i2c::{self, Config as I2cConfig, I2c},
    peripherals::{self},
    time::Hertz,
};
use embassy_time::{Duration, Ticker, Timer};
use embedded_graphics::{
    mono_font::{MonoTextStyleBuilder, ascii::FONT_6X10},
    pixelcolor::BinaryColor,
    prelude::*,
    text::{Baseline, Text},
};
use heapless::String;
use ssd1306::{
    I2CDisplayInterface, Ssd1306Async, mode::DisplayConfigAsync, rotation::DisplayRotation,
    size::DisplaySize128x64,
};

use {defmt_rtt as _, panic_probe as _};

// Bind I2C interrupts.
//
// Embassy's I2C driver relies on interrupts and DMA; these must be defined here.\
bind_interrupts!(struct Irqs {
    I2C1_EV => i2c::EventInterruptHandler<peripherals::I2C1>;
    I2C1_ER => i2c::ErrorInterruptHandler<peripherals::I2C1>;
});

#[embassy_executor::main]
async fn main(spawner: Spawner) {
    let p = embassy_stm32::init(clock_config());
    info!("System initialized");

    // Spawn LED blinking task
    let led = Output::new(p.PC0, Level::High, Speed::Low);
    spawner.spawn(blinky(led)).unwrap();

    // Configure and spawn OLED task
    let i2c = configure_i2c(p.I2C1, p.PB6, p.PB7, p.DMA1_CH6, p.DMA1_CH0);
    spawner.spawn(oled_task(i2c)).unwrap();

    // Main loop does nothing but sleep
    loop {
        Timer::after(Duration::from_secs(1)).await;
    }
}

/// Blink an LED every 300 ms.
#[embassy_executor::task]
async fn blinky(mut led: Output<'static>) {
    let mut ticker = Ticker::every(Duration::from_millis(300));
    loop {
        led.toggle();
        ticker.next().await;
    }
}

/// OLED display task using I2C with DMA.
///
/// This task initializes an SSD1306 display and periodically updates it with
/// text and a counter. The display updates are performed asynchronously via DMA.
#[embassy_executor::task]
async fn oled_task(i2c: I2c<'static, embassy_stm32::mode::Async, embassy_stm32::i2c::Master>) {
    let mut display = match create_display(i2c).await {
        Ok(display) => display,
        Err(_) => {
            error!("Failed to initialize OLED");
            return;
        }
    };

    let text_style = MonoTextStyleBuilder::new()
        .font(&FONT_6X10)
        .text_color(BinaryColor::On)
        .build();

    let mut counter = 0u32;
    let mut ticker = embassy_time::Ticker::every(embassy_time::Duration::from_millis(100));
    loop {
        update_display(&mut display, &text_style, counter).await;
        counter = counter.wrapping_add(1);
        ticker.next().await;
        info!("Display updated: {}", counter);
    }
}

/// Create and initialize an SSD1306 display.
async fn create_display(
    i2c: I2c<'static, embassy_stm32::mode::Async, embassy_stm32::i2c::Master>,
) -> Result<
    Ssd1306Async<
        ssd1306::prelude::I2CInterface<
            embassy_stm32::i2c::I2c<
                'static,
                embassy_stm32::mode::Async,
                embassy_stm32::i2c::Master,
            >,
        >,
        DisplaySize128x64,
        ssd1306::mode::BufferedGraphicsModeAsync<DisplaySize128x64>,
    >,
    (),
> {
    let interface = I2CDisplayInterface::new(i2c);
    let mut display = Ssd1306Async::new(interface, DisplaySize128x64, DisplayRotation::Rotate0)
        .into_buffered_graphics_mode();

    display.init().await.map_err(|_| ())?;
    Ok(display)
}

/// Update the display with current information.
async fn update_display(
    display: &mut Ssd1306Async<
        ssd1306::prelude::I2CInterface<
            embassy_stm32::i2c::I2c<
                'static,
                embassy_stm32::mode::Async,
                embassy_stm32::i2c::Master,
            >,
        >,
        DisplaySize128x64,
        ssd1306::mode::BufferedGraphicsModeAsync<DisplaySize128x64>,
    >,
    text_style: &embedded_graphics::mono_font::MonoTextStyle<'_, BinaryColor>,
    counter: u32,
) {
    display.clear(BinaryColor::Off).unwrap();

    Text::with_baseline(
        "STM32F4 Embassy",
        Point::new(0, 0),
        *text_style,
        Baseline::Top,
    )
    .draw(display)
    .unwrap();

    Text::with_baseline(
        "Mode: DMA Async",
        Point::new(0, 16),
        *text_style,
        Baseline::Top,
    )
    .draw(display)
    .unwrap();

    let mut buf = String::<32>::new();
    core::write!(&mut buf, "Count: {}", counter).unwrap();

    Text::with_baseline(&buf, Point::new(0, 32), *text_style, Baseline::Top)
        .draw(display)
        .unwrap();

    if let Err(_) = display.flush().await {
        error!("OLED flush failed");
    }
}

/// Configure I2C1 with DMA for OLED communication.
fn configure_i2c(
    i2c: Peri<'static, peripherals::I2C1>,
    scl: Peri<'static, peripherals::PB6>,
    sda: Peri<'static, peripherals::PB7>,
    dma_tx: Peri<'static, peripherals::DMA1_CH6>,
    dma_rx: Peri<'static, peripherals::DMA1_CH0>,
) -> I2c<'static, embassy_stm32::mode::Async, embassy_stm32::i2c::Master> {
    let mut config = I2cConfig::default();
    config.frequency = Hertz(1_000_000); // 1 MHz
    config.gpio_speed = Speed::High;
    config.scl_pullup = true;
    config.sda_pullup = true;

    I2c::new(i2c, scl, sda, Irqs, dma_tx, dma_rx, config)
}

/// Generate clock configuration for maximum frequency (168 MHz).
fn clock_config() -> embassy_stm32::Config {
    use embassy_stm32::rcc::*;
    use embassy_stm32::time::*;

    let mut config = embassy_stm32::Config::default();
    config.rcc.hse = Some(Hse {
        freq: Hertz::mhz(8),
        mode: HseMode::Oscillator,
    });
    config.rcc.pll_src = PllSource::HSE;
    config.rcc.pll = Some(Pll {
        prediv: PllPreDiv::DIV4,
        mul: PllMul::MUL168,
        divp: Some(PllPDiv::DIV2),
        divq: Some(PllQDiv::DIV7),
        divr: None,
    });
    config.rcc.sys = Sysclk::PLL1_P;
    config.rcc.ahb_pre = AHBPrescaler::DIV1;
    config.rcc.apb1_pre = APBPrescaler::DIV4;
    config.rcc.apb2_pre = APBPrescaler::DIV2;
    config.enable_debug_during_sleep = true;
    config
}
