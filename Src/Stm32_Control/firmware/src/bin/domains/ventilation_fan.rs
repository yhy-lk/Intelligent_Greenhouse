// #![no_std]

use bsw::motor_ctrl::FanMotor;
use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_time::{Duration, Ticker};

// 物理控制环周期定标：100ms (10Hz)
const LOOP_INTERVAL_MS: u64 = 100;
const DT_SEC: f32 = LOOP_INTERVAL_MS as f32 / 1000.0;

/// 补风电机闭环控制任务
/// 负责以 10Hz 的频率调度底层的测速解算与 PID 输出，并联通全局状态单例
#[embassy_executor::task]
pub async fn ventilation_fan_task(mut motor: FanMotor<'static>) {
    trace!("Starting Ventilation Fan Control Task (10Hz)");

    // 硬件状态对齐：在控制环启动前记录编码器基准绝对值，避免第一帧解算出巨大的溢出误差
    motor.sync_encoder();

    let mut ticker = Ticker::every(Duration::from_millis(LOOP_INTERVAL_MS));

    loop {
        // 1. 获取目标状态与控制参数 (极短临界区 1：数据读取)
        let (power_switch, mut target_rpm, kp, ki, kd) = {
            let state = GLOBAL_STATE.lock().await;
            (
                state.target.ventilation_fan.unwrap_or(false),
                state.target.target_fan_speed_rpm.unwrap_or(0) as f32,
                state.target.pid_p.unwrap_or(1.0), // 缺省比例系数
                state.target.pid_i.unwrap_or(0.1), // 缺省积分系数
                state.target.pid_d.unwrap_or(0.0), // 缺省微分系数
            )
        };

        // 逻辑越权防错：如果主开关为 OFF，直接覆写目标转速为 0.0 制动
        if !power_switch {
            target_rpm = 0.0;
        }

        // 2. 硬件层执行闭环算法
        // 底层完成：读取硬件 QEI -> 溢出解算 RPM -> PID 运算 -> 限幅 -> TB6612 PWM 下发
        motor.control_tick(target_rpm, DT_SEC, kp, ki, kd);

        // 3. 提取底层解算出的当前真实物理量
        let actual_rpm = motor.get_rpm();

        // 4. 更新系统当前状态 (极短临界区 2：数据写入)
        {
            let mut state = GLOBAL_STATE.lock().await;
            // 数值规整：处理由于传感器毛刺或制动时产生的极小负值，统一输出给总线为 >= 0 的 u32
            state.current.fan_speed_rpm = Some(actual_rpm.max(0.0) as u32);
        }

        // 5. 严格的时间节拍挂起
        ticker.next().await;
    }
}