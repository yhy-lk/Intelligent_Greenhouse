// #![no_std]

use crate::shared::GLOBAL_STATE;
use bsw::motor_ctrl::FanMotor;
use defmt::*;
use embassy_time::{Duration, Ticker};

const LOOP_INTERVAL_MS: u64 = 100;
const DT_SEC: f32 = LOOP_INTERVAL_MS as f32 / 1000.0;
const DEFAULT_FAN_PID_KP: f32 = 2.0;
const DEFAULT_FAN_PID_KI: f32 = 0.5;
const DEFAULT_FAN_PID_KD: f32 = 0.0;

#[embassy_executor::task]
pub async fn ventilation_fan_task(mut motor: FanMotor<'static>) {
    trace!("Starting Ventilation Fan Control Task (10Hz)");

    motor.sync_encoder();

    let mut ticker = Ticker::every(Duration::from_millis(LOOP_INTERVAL_MS));
    let mut last_power_switch: Option<bool> = None;
    let mut last_target_rpm: Option<f32> = None;
    let mut last_kp: Option<f32> = None;
    let mut last_ki: Option<f32> = None;
    let mut last_kd: Option<f32> = None;
    let mut last_actual_rpm_raw: Option<f32> = None;
    let mut last_actual_rpm_reported: Option<u32> = None;

    loop {
        let (power_switch, mut target_rpm, kp, ki, kd) = {
            let state = GLOBAL_STATE.lock().await;
            (
                state.target.ventilation_fan.unwrap_or(false),
                state.target.target_fan_speed_rpm.unwrap_or(0) as f32,
                state.target.pid_p.unwrap_or(DEFAULT_FAN_PID_KP),
                state.target.pid_i.unwrap_or(DEFAULT_FAN_PID_KI),
                state.target.pid_d.unwrap_or(DEFAULT_FAN_PID_KD),
            )
        };

        if !power_switch {
            target_rpm = 0.0;
        }

        if last_power_switch != Some(power_switch) {
            info!("ventilation_fan.power_switch -> {}", power_switch);
            last_power_switch = Some(power_switch);
        }
        if last_target_rpm != Some(target_rpm) {
            info!("ventilation_fan.target_rpm -> {}", target_rpm);
            last_target_rpm = Some(target_rpm);
        }
        if last_kp != Some(kp) {
            info!("ventilation_fan.pid_kp -> {}", kp);
            last_kp = Some(kp);
        }
        if last_ki != Some(ki) {
            info!("ventilation_fan.pid_ki -> {}", ki);
            last_ki = Some(ki);
        }
        if last_kd != Some(kd) {
            info!("ventilation_fan.pid_kd -> {}", kd);
            last_kd = Some(kd);
        }

        motor.control_tick(target_rpm, DT_SEC, kp, ki, kd);

        let actual_rpm = motor.get_rpm();
        let actual_rpm_reported = actual_rpm.max(0.0) as u32;
        let pid_error = motor.get_pid_error();
        let pid_integral = motor.get_pid_integral();
        let pid_integral_limit = motor.get_pid_integral_limit();
        let pid_derivative = motor.get_pid_derivative();
        let pid_output = motor.get_pid_output();
        let pid_output_limit = motor.get_pid_output_limit();
        let pwm_command = motor.get_last_pwm_command();
        let pwm_duty = motor.get_last_pwm_duty();
        let max_duty = motor.get_max_duty();

        if last_actual_rpm_raw != Some(actual_rpm) {
            debug!("ventilation_fan.actual_rpm_raw -> {}", actual_rpm);
            last_actual_rpm_raw = Some(actual_rpm);
        }
        if last_actual_rpm_reported != Some(actual_rpm_reported) {
            debug!("ventilation_fan.actual_rpm -> {}", actual_rpm_reported);
            last_actual_rpm_reported = Some(actual_rpm_reported);
        }

        debug!(
            "ventilation_fan.ctrl target={} actual={} error={} integral={}/{} derivative={} pid_output={}/{} pwm_command={} pwm_duty={}/{}",
            target_rpm,
            actual_rpm,
            pid_error,
            pid_integral,
            pid_integral_limit,
            pid_derivative,
            pid_output,
            pid_output_limit,
            pwm_command,
            pwm_duty,
            max_duty
        );

        {
            let mut state = GLOBAL_STATE.lock().await;
            state.current.fan_speed_rpm = Some(actual_rpm_reported);
        }

        ticker.next().await;
    }
}
