use defmt::*;
use embassy_stm32::peripherals::TIM5;
use embassy_stm32::timer::simple_pwm::SimplePwm;
use embassy_time::{Duration, Timer};
use crate::shared::GLOBAL_STATE;

// 预计算常量，消除运行时浮点运算
// 映射关系: 0° = 500us, 300° = 2500us
const PULSE_US_0_DEG: u32 = 500;
const PULSE_US_90_DEG: u32 = 1100; // 500 + (90 / 300) * 2000
const PWM_PERIOD_US: u32 = 20000;

#[embassy_executor::task]
pub async fn mg995_task(
    mut pwm: SimplePwm<'static, TIM5>
) {
    let mut ch2 = pwm.ch2();
    ch2.enable();
    trace!("PWM Initialized at 50Hz on PA1 (TIM5_CH2)");

    // 使用 Option 记录当前物理状态，None 表示系统刚启动，尚未输出任何 PWM
    let mut current_state: Option<bool> = None;

    loop {
        // 1. 获取目标状态 (锁的生命周期仅在此语句块内)
        let target_state = { GLOBAL_STATE.lock().await.target.window_actuator.unwrap_or(false) };

        // 2. 状态发生变化，或首次初始化时才执行寄存器写入
        if Some(target_state) != current_state {
            let pulse_us = if target_state {
                PULSE_US_90_DEG
            } else {
                PULSE_US_0_DEG
            };

            ch2.set_duty_cycle_fraction(pulse_us, PWM_PERIOD_US);
            trace!("Window Actuator State Update: {} -> Pulse: {} us", target_state, pulse_us);

            current_state = Some(target_state);

            // 给舵机预留物理转动时间 (假设 MG995 走 90度 约需 1.5秒)
            // 在此期间不接受新指令，防止频繁反转引起瞬态大电流
            Timer::after(Duration::from_millis(1500)).await;
        } else {
            // 3. 状态未变，以 50ms (20Hz) 的频率进行短周期轮询，保证极速响应
            Timer::after(Duration::from_millis(50)).await;
        }
    }
}