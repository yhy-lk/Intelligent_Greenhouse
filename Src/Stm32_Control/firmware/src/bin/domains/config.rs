// use crate::*;

pub struct SystemConfig;

impl SystemConfig {
    pub fn system_clock_config() -> embassy_stm32::Config {
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

    pub fn i2c1_config() -> embassy_stm32::i2c::Config {
        let mut config = embassy_stm32::i2c::Config::default();
        config.frequency = embassy_stm32::time::Hertz(400_000);
        config.gpio_speed = embassy_stm32::gpio::Speed::High;
        config.scl_pullup = true;
        config.sda_pullup = true;

        config
    }

    pub fn i2c2_config() -> embassy_stm32::i2c::Config {
        let mut config = embassy_stm32::i2c::Config::default();
        config.frequency = embassy_stm32::time::Hertz(400_000);
        config.gpio_speed = embassy_stm32::gpio::Speed::High;
        config.scl_pullup = true;
        config.sda_pullup = true;

        config
    }

}
