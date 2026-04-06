use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_stm32::gpio::Output;
use embassy_time::{Duration, Timer};

#[embassy_executor::task]
pub async fn water_pump_task(mut pump_pin: Output<'static>) {
    trace!("Starting Water Pump task");

    // 硬件初始化防错：系统启动时强制引脚拉低，确保继电器/MOS管处于断开状态
    pump_pin.set_low();
    let mut current_state = false;

    loop {
        // 1. 获取目标状态 (锁生命周期由单行语句结束的分号自动控制)
        let target_state = GLOBAL_STATE.lock().await.target.water_pump.unwrap_or(false);

        // 2. 边缘检测与硬件动作
        if target_state != current_state {
            if target_state {
                pump_pin.set_high();
                info!("Water Pump State Update: ON");
            } else {
                pump_pin.set_low();
                info!("Water Pump State Update: OFF");
            }
            current_state = target_state;
        }

        // 3. 100ms 周期轮询 (10Hz)，保证控制指令的高响应性且极低资源开销
        Timer::after(Duration::from_millis(100)).await;
    }
}