
use embassy_stm32::{
    gpio::Output,
    // i2c::{I2c, Master},
    // mode::{Async},
};

pub struct SystemLed<'d> {
    pub pin: Output<'d>,
    pub toggle_count: u32,
}

// 实现 service 接口
impl<'d> service::led::BoardLed for SystemLed<'d> {
    fn on(&mut self) {
        if self.pin.is_set_low() {
            return;
        }
        self.pin.set_high();
        self.toggle_count += 1;
    }

    fn off(&mut self) {
        if self.pin.is_set_high() {
            return;
        }
        self.pin.set_low();
        self.toggle_count += 1;
    }

    fn toggle(&mut self) {
        self.pin.toggle();
        self.toggle_count += 1;
    }

    fn toggle_count(&mut self) -> u32 {
        self.toggle_count
    }
}

