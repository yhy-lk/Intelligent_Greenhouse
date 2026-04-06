#![no_std]
#![no_main]

// 全文采用中文注释

use defmt::*;
use embassy_executor::Spawner;
use embassy_stm32::gpio::{Level, Output, Speed};
use embassy_stm32::i2c::{self, I2c};
use embassy_stm32::time::Hertz;
// use embassy_stm32::peripherals::I2C1;
// use embassy_stm32::time::Hertz;
use embassy_stm32::{bind_interrupts, peripherals};
use embassy_time::{Duration, Timer};
use embedded_graphics::{
    mono_font::{MonoTextStyleBuilder, ascii::FONT_6X10},
    pixelcolor::BinaryColor,
    prelude::*,
    text::{Baseline, Text},
};
// use ssd1306::{prelude::*, Ssd1306};
use {defmt_rtt as _, panic_probe as _};

// 1. 绑定 I2C 中断
// Embassy 的 I2C 驱动是基于中断和 DMA 协作的，必须在此定义
bind_interrupts!(struct Irqs {
    I2C1_EV => i2c::EventInterruptHandler<peripherals::I2C1>;
    I2C1_ER => i2c::ErrorInterruptHandler<peripherals::I2C1>;
});

#[embassy_executor::main]
async fn main(spawner: Spawner) {
    // 初始化系统时钟
    let p = embassy_stm32::init(clock_config());

    info!("系统启动完成");

    // --- LED 配置 (保持你原有的逻辑) ---
    // 注意：你之前的 IO 表中 PC0 是土壤湿度传感器，这里暂时按你提供的代码用作 LED
    let led_pin = Output::new(p.PC0, Level::High, Speed::Low);
    spawner.spawn(blinky_task(led_pin)).unwrap();

    // --- OLED 配置 (I2C1 + DMA) ---
    // 物理连接: PB6 -> SCL, PB7 -> SDA
    // F407 DMA映射: I2C1_TX 使用 DMA1_STREAM6, I2C1_RX 使用 DMA1_STREAM0
    let mut i2c_config = i2c::Config::default();
    {
        i2c_config.frequency = Hertz(1_000_000); // 1MHz
        i2c_config.gpio_speed = Speed::High;
        i2c_config.scl_pullup = true; // 启用内部上拉
        i2c_config.sda_pullup = true; // 启用内部上拉
    }
    // 创建基于 DMA 的 I2C 驱动
    // 注意：这里传入了 DMA 通道，Embassy 会自动开启 DMA 模式
    let i2c = I2c::new(
        p.I2C1, p.PB6, p.PB7, Irqs, p.DMA1_CH6, // 发送 DMA
        p.DMA1_CH0, // 接收 DMA (虽然 OLED 主要是写，但最好都配上)
        i2c_config,
    );

    // 启动 OLED 显示任务
    // 将 i2c 总线的所有权移交给该任务
    spawner.spawn(oled_task(i2c)).unwrap();

    loop {
        // 主循环可以做一些低频监控，或者休眠
        Timer::after(Duration::from_secs(1)).await;
    }
}

// LED闪烁任务，每隔300毫秒切换一次状态
#[embassy_executor::task]
async fn blinky_task(mut led: Output<'static>) {
    use embassy_time::*;
    let mut ticker = Ticker::every(Duration::from_millis(300));
    loop {
        led.toggle();
        ticker.next().await;
    }
}

// OLED 显示任务
// 这是一个 Async 任务，使用 DMA 刷新屏幕时不会阻塞 CPU
#[embassy_executor::task]
async fn oled_task(i2c: I2c<'static, embassy_stm32::mode::Async, embassy_stm32::i2c::Master>) {
    // 1. 初始化 SSD1306 驱动
    // I2CDisplayInterface 是 ssd1306 crate 提供的接口适配层
    use ssd1306::mode::DisplayConfigAsync;
    use ssd1306::rotation::DisplayRotation;
    use ssd1306::size::DisplaySize128x64;
    use ssd1306::*;
    let interface = I2CDisplayInterface::new(i2c);
    let mut display_i2c = Ssd1306Async::new(interface, DisplaySize128x64, DisplayRotation::Rotate0)
        .into_buffered_graphics_mode();

    // 2. 硬件初始化序列 (发送开机命令)
    if let Err(_e) = display_i2c.init().await {
        error!("error initializing OLED");
        return;
    }

    // 3. 定义文本样式
    let text_style = MonoTextStyleBuilder::new()
        .font(&FONT_6X10)
        .text_color(BinaryColor::On)
        .build();

    let mut counter = 0;

    loop {
        // 清空缓冲区 (黑色)
        display_i2c.clear(BinaryColor::Off).unwrap();

        // 绘制静态标题
        Text::with_baseline(
            "STM32F4 Embassy",
            Point::new(0, 0),
            text_style,
            Baseline::Top,
        )
        .draw(&mut display_i2c)
        .unwrap();

        // 绘制 DMA 状态
        Text::with_baseline(
            "Mode: DMA Async",
            Point::new(0, 16),
            text_style,
            Baseline::Top,
        )
        .draw(&mut display_i2c)
        .unwrap();

        // 绘制动态计数器
        use core::fmt::Write;
        let mut buf = heapless::String::<32>::new();
        core::write!(&mut buf, "Count: {}", counter).unwrap();

        Text::with_baseline(&buf, Point::new(0, 32), text_style, Baseline::Top)
            .draw(&mut display_i2c)
            .unwrap();

        // ⚠️ 关键步骤：Flush
        // 这一步会将 framebuffer 的数据通过 I2C DMA 发送给屏幕。
        // 因为我们加了 .await，CPU 会在这里挂起当前任务去处理 blinky_task，
        // 而 DMA 控制器会在后台默默搬运数据。
        if let Err(_) = display_i2c.flush().await {
            error!("OLED 刷新失败");
        }

        counter += 1;
        Timer::after(Duration::from_millis(100)).await;
    }
}

// 按照Rust文档格式写注释
/// 配置时钟为官方最高频率 (168MHz)
fn clock_config() -> embassy_stm32::Config {
    let mut config = embassy_stm32::Config::default();
    {
        use embassy_stm32::rcc::*;
        use embassy_stm32::time::*;

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
    }
    config
}
