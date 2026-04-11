#![no_std]
#![no_main]

// use defmt::info;
use embassy_executor::Spawner;
use embassy_stm32::{
    bind_interrupts,
    gpio,
    i2c,
    time,
    // mode,
    timer,
    peripherals,
};

use embassy_stm32::can::{
    Can, Fifo, Rx0InterruptHandler, Rx1InterruptHandler, SceInterruptHandler, TxInterruptHandler,
};

use embassy_sync::{
    blocking_mutex,
    mutex,
    // signal::Signal,
};
use ssd1306::{
    I2CDisplayInterface, Ssd1306Async, mode::DisplayConfigAsync, rotation::DisplayRotation,
    size::DisplaySize128x64,
};

use embassy_embedded_hal::shared_bus;
use {defmt_rtt as _, panic_probe as _};

bind_interrupts!(struct Irqs {
    I2C1_EV => i2c::EventInterruptHandler<peripherals::I2C1>;
    I2C1_ER => i2c::ErrorInterruptHandler<peripherals::I2C1>;

    // ================= 新增 CAN1 中断 =================
    CAN1_RX0 => Rx0InterruptHandler<peripherals::CAN1>;
    CAN1_RX1 => Rx1InterruptHandler<peripherals::CAN1>;
    CAN1_SCE => SceInterruptHandler<peripherals::CAN1>;
    CAN1_TX => TxInterruptHandler<peripherals::CAN1>;
});

// 引入我们的层
// use app;
use bsw;
// use rte;

mod domains;
use crate::domains::{
    board_led, 
    ui, 
    config::SystemConfig, 
    ws2812, 
    dispatcher,
    sht30,
    soil_moisture,
    mg995,
    bh1750,
    water_pump,
    humidifier,
    can_rx, // 新增
    can_tx, // 新增
    ventilation_fan,
    hw_test,
};

mod shared;
use shared::{
    StaticCellAsyncMasterI2cBus, StaticCellBlockingMasterI2cBus, LedSingal,
    // Sht30, ConcreteDisplay, Ws2812, SystemLed
};
use static_cell::StaticCell;
use embassy_sync::signal::Signal;
use embassy_sync::channel::Channel;
use embassy_sync::blocking_mutex::raw::{ThreadModeRawMutex};
use smart_leds::RGB8;

pub static I2C1_BUS: StaticCellAsyncMasterI2cBus = StaticCell::new();
pub static I2C2_BUS: StaticCellBlockingMasterI2cBus = StaticCell::new();
pub static LED_TOGGLE_COUNT: LedSingal = Signal::new();
static FILL_LIGHT_CHANNEL: Channel<ThreadModeRawMutex, RGB8, 10> = Channel::new();


#[embassy_executor::main]
async fn main(spawner: Spawner) {
    // 1. 初始化 MCU (BSW 内部细节)
    let p = embassy_stm32::init(SystemConfig::system_clock_config());

    // 2. 组装 LED (BSW -> RTE)
    let led_pin = gpio::Output::new(p.PC0, gpio::Level::High, gpio::Speed::Low);
    let my_led = bsw::led::SystemLed {
        pin: led_pin,
        toggle_count: 0,
    };
    spawner.spawn(board_led::led_toggle_task(my_led, &LED_TOGGLE_COUNT)).unwrap();

    // 3. 组装 OLED (BSW -> RTE)
    let i2c1 = i2c::I2c::new(
        p.I2C1,
        p.PB6,
        p.PB7,
        Irqs,
        p.DMA1_CH7,
        p.DMA1_CH0,
        SystemConfig::i2c1_config(),
    );
    let i2c1_bus = I2C1_BUS.init(mutex::Mutex::new(i2c1));

    let oled_i2c = shared_bus::asynch::i2c::I2cDevice::new(i2c1_bus);
    let interface = I2CDisplayInterface::new(oled_i2c);
    let mut display = Ssd1306Async::new(interface, DisplaySize128x64, DisplayRotation::Rotate0)
        .into_buffered_graphics_mode();
    display.init()
        .await
        .inspect(|_| defmt::info!("oled display init success"))
        .inspect_err(|_| defmt::error!("oled display init error"))
        .ok();
    // 这里的 display 既是具体的 SSD1306，也自动实现了 embedded-graphics DrawTarget
    spawner.spawn(ui::oled_ui_task(display, &LED_TOGGLE_COUNT)).unwrap();

    let ws2812_pwm = timer::simple_pwm::SimplePwm::new(
        p.TIM4,
        Some(timer::simple_pwm::PwmPin::new(p.PD12, gpio::OutputType::PushPull)),
        None,
        None,
        None,
        time::khz(800), // data rate of ws2812
        timer::low_level::CountingMode::EdgeAlignedUp,
    );

    let ws2812 = bsw::ws2812::Ws2812::new(ws2812_pwm, p.DMA1_CH6, timer::Channel::Ch1);
    spawner.spawn(ws2812::fill_light_task(ws2812)).unwrap();

    let i2c2: i2c::I2c<'_, embassy_stm32::mode::Blocking, i2c::Master> = i2c::I2c::new_blocking(
        p.I2C2, 
        p.PB10, 
        p.PB11, 
        SystemConfig::i2c2_config()
    );

    let i2c2_bus_mutex = blocking_mutex::Mutex::new(core::cell::RefCell::new(i2c2));
    let i2c2_bus = I2C2_BUS.init(i2c2_bus_mutex);

    let sht30_i2c= shared_bus::blocking::i2c::I2cDevice::new(i2c2_bus);
    spawner.spawn(sht30::sht30_task(sht30_i2c)).unwrap();

    let bh1750_i2c = shared_bus::blocking::i2c::I2cDevice::new(i2c2_bus);
    spawner.spawn(bh1750::bh1750_task(bh1750_i2c)).unwrap();

    spawner.spawn(dispatcher::dispatcher_task(FILL_LIGHT_CHANNEL.sender())).unwrap();


    let mut adc_config = embassy_stm32::adc::AdcConfig::default();
    adc_config.resolution = Some(embassy_stm32::adc::Resolution::BITS12);
    let adc = embassy_stm32::adc::Adc::new_with_config(p.ADC1, adc_config);

    spawner.spawn(soil_moisture::soil_moisture_task(adc, p.PC1)).unwrap();

    // 配置 PA1 为复用推挽输出，供 PWM 使用

    let ch2_pin = embassy_stm32::timer::simple_pwm::PwmPin::new(p.PA1, gpio::OutputType::PushPull);

    // 初始化 TIM5 的 PWM
    // 注意坑点：PA1 对应的是 Channel 2，所以 Some(ch2_pin) 必须放在第二个参数位置
    let pwm = embassy_stm32::timer::simple_pwm::SimplePwm::new(
        p.TIM5,
        None,           // CH1
        Some(ch2_pin),  // CH2 (PA1)
        None,           // CH3
        None,           // CH4
        embassy_stm32::time::hz(50),         // 舵机标准频率：50Hz
        Default::default(),
    );

    spawner.spawn(mg995::mg995_task(pwm)).unwrap();

    let water_pump_pin = gpio::Output::new(p.PD13, gpio::Level::Low, gpio::Speed::Low);
    spawner.spawn(water_pump::water_pump_task(water_pump_pin)).unwrap();

    let humidifier_pin = gpio::Output::new(p.PE4, gpio::Level::Low, gpio::Speed::Low);
    spawner.spawn(humidifier::humidifier_task(humidifier_pin)).unwrap();

    // ================= 新增 补风电机 (Fan Motor) 初始化 =================
    
    // 1. 配置 PWM (TIM1_CH1 -> PE9)
    // 频率设定为 20kHz，处于人耳听觉范围之上，可消除电机高频啸叫
    let fan_pwm_pin = embassy_stm32::timer::simple_pwm::PwmPin::new(p.PE9, gpio::OutputType::PushPull);
    let fan_pwm = embassy_stm32::timer::simple_pwm::SimplePwm::new(
        p.TIM1,
        Some(fan_pwm_pin),
        None,
        None,
        None,
        time::khz(20), 
        Default::default(),
    );

    // 2. 配置方向控制引脚 (PE10, PE11)
    let dir_in1 = gpio::Output::new(p.PE10, gpio::Level::Low, gpio::Speed::Low);
    let dir_in2 = gpio::Output::new(p.PE11, gpio::Level::Low, gpio::Speed::Low);

    // 3. 配置 QEI 正交编码器接口 (TIM3, CH1: PC6, CH2: PC7)
    let qei = embassy_stm32::timer::qei::Qei::new(p.TIM3, p.PC6, p.PC7, embassy_stm32::timer::qei::Config::default());

    // 4. 组装 FanMotor 硬件层对象
    let fan_motor = bsw::motor_ctrl::FanMotor::new(fan_pwm, dir_in1, dir_in2, qei);

    // 5. 挂载补风电机闭环控制任务
    spawner.spawn(ventilation_fan::ventilation_fan_task(fan_motor)).unwrap();


    
    // 1. 初始化 CAN 外设 (RX: PB8, TX: PB9)
    let mut can = Can::new(p.CAN1, p.PB8, p.PB9, Irqs);

    // 2. 获取并应用协议层的硬件过滤器配置
    let filters = bsw::can_proto::get_can_filters();
    can.modify_filters()
        .enable_bank(0, Fifo::Fifo0, filters[0])  // 绑定广播地址过滤器到 FIFO0
        .enable_bank(1, Fifo::Fifo0, filters[1]); // 绑定本机地址过滤器到 FIFO0

    // 3. 配置波特率 (1 Mbps) 并生效
    can.modify_config()
        // .set_loopback(true) // 若前期没有接真实收发器和总线，需解开此注释开启回环测试
        // .set_silent(true)
        .set_bitrate(1_000_000);
    can.enable().await;

    // 4. 将 CAN 实例拆分为独立的 发送半边 (Tx) 和 接收半边 (Rx)
    let (can_tx_handle, can_rx_handle) = can.split();

    // 5. 挂载收发调度任务
    spawner.spawn(can_rx::can_rx_task(can_rx_handle)).unwrap();
    spawner.spawn(can_tx::can_tx_task(can_tx_handle)).unwrap(); 

    // spawner.spawn(hw_test::hardware_smoke_test_task()).unwrap();

}



