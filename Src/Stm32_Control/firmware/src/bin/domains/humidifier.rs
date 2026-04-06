use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_stm32::gpio::Output;
use embassy_time::{Duration, Timer};

#[embassy_executor::task]
pub async fn humidifier_task(mut humidifier_pin: Output<'static>) {
    trace!("Starting Humidifier task");

    // 硬件初始化防错：系统启动时强制引脚拉低
    humidifier_pin.set_low();
    let mut current_state = false;

    loop {
        // 1. 获取目标状态
        let target_state = GLOBAL_STATE.lock().await.target.humidifier.unwrap_or(false);

        // 2. 边缘检测与硬件动作
        if target_state != current_state {
            if target_state {
                humidifier_pin.set_high();
                info!("Humidifier State Update: ON");
            } else {
                humidifier_pin.set_low();
                info!("Humidifier State Update: OFF");
            }
            current_state = target_state;
        }

        // 3. 100ms 周期轮询
        Timer::after(Duration::from_millis(100)).await;
    }
}