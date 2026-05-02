use crate::shared::GLOBAL_STATE;
use bsw::protocol::ControlMode;
use defmt::*;
use embassy_time::{Duration, Ticker};

const DISPATCH_INTERVAL_MS: u64 = 500;

const TEMP_FAN_ON_MARGIN_C: f32 = 1.0;
const TEMP_FAN_OFF_MARGIN_C: f32 = 0.3;
const AIR_HUM_FAN_ON_MARGIN: f32 = 8.0;
const AIR_HUM_FAN_OFF_MARGIN: f32 = 3.0;

const AIR_HUM_HUMIDIFIER_ON_MARGIN: f32 = 5.0;
const AIR_HUM_HUMIDIFIER_OFF_MARGIN: f32 = 2.0;

const SOIL_HUM_PUMP_ON_MARGIN: f32 = 5.0;
const SOIL_HUM_PUMP_OFF_MARGIN: f32 = 2.0;

const LIGHT_OFF_MARGIN_LUX: f32 = 100.0;
const LIGHT_PWM_MIN_ACTIVE_DUTY: u32 = 8;
const LIGHT_PWM_STEP_UP: u32 = 1;
const LIGHT_PWM_STEP_DOWN: u32 = 1;

const AUTO_FAN_MIN_RPM: u32 = 1200;
const AUTO_FAN_MAX_RPM: u32 = 4500;

#[derive(Clone, Copy, Debug, PartialEq)]
struct AutoControlOutputs {
    ventilation_fan: bool,
    target_fan_speed_rpm: u32,
    humidifier: bool,
    water_pump: bool,
    light_main_power: bool,
    light_pwm_duty: u32,
}

impl AutoControlOutputs {
    const fn all_off() -> Self {
        Self {
            ventilation_fan: false,
            target_fan_speed_rpm: 0,
            humidifier: false,
            water_pump: false,
            light_main_power: false,
            light_pwm_duty: 0,
        }
    }
}

#[embassy_executor::task]
pub async fn dispatcher_task() {
    info!("Starting auto dispatcher task");

    let mut ticker = Ticker::every(Duration::from_millis(DISPATCH_INTERVAL_MS));
    let mut last_auto_outputs = AutoControlOutputs::all_off();

    loop {
        let (
            control_mode,
            temperature,
            humidity_air,
            humidity_soil,
            light_intensity,
            target_temperature,
            target_humidity_air,
            target_humidity_soil,
            target_light_intensity,
        ) = {
            let state = GLOBAL_STATE.lock().await;
            (
                state.target.control_mode.unwrap_or(ControlMode::Manual),
                state.current.temperature,
                state.current.humidity_air,
                state.current.humidity_soil,
                state.current.light_intensity,
                state.target.target_temperature,
                state.target.target_humidity_air,
                state.target.target_humidity_soil,
                state.target.target_light_intensity,
            )
        };

        if control_mode != ControlMode::Auto {
            last_auto_outputs = AutoControlOutputs::all_off();
            ticker.next().await;
            continue;
        }

        let fan_required_by_temp = match (temperature, target_temperature) {
            (Some(current), Some(target)) => {
                hysteresis_high_side(current, target, TEMP_FAN_ON_MARGIN_C, TEMP_FAN_OFF_MARGIN_C, last_auto_outputs.ventilation_fan)
            }
            _ => false,
        };

        let fan_required_by_humidity = match (humidity_air, target_humidity_air) {
            (Some(current), Some(target)) => {
                hysteresis_high_side(current, target, AIR_HUM_FAN_ON_MARGIN, AIR_HUM_FAN_OFF_MARGIN, last_auto_outputs.ventilation_fan)
            }
            _ => false,
        };

        let ventilation_fan = fan_required_by_temp || fan_required_by_humidity;

        let target_fan_speed_rpm = if ventilation_fan {
            compute_fan_speed_rpm(temperature, target_temperature, humidity_air, target_humidity_air)
        } else {
            0
        };

        let humidifier = match (humidity_air, target_humidity_air) {
            (Some(current), Some(target)) => hysteresis_low_side(
                current,
                target,
                AIR_HUM_HUMIDIFIER_ON_MARGIN,
                AIR_HUM_HUMIDIFIER_OFF_MARGIN,
                last_auto_outputs.humidifier,
            ),
            _ => false,
        };

        let water_pump = match (humidity_soil, target_humidity_soil) {
            (Some(current), Some(target)) => hysteresis_low_side(
                current,
                target,
                SOIL_HUM_PUMP_ON_MARGIN,
                SOIL_HUM_PUMP_OFF_MARGIN,
                last_auto_outputs.water_pump,
            ),
            _ => false,
        };

        let light_pwm_duty = compute_light_pwm_duty(
            light_intensity,
            target_light_intensity,
            last_auto_outputs.light_pwm_duty,
        );
        let light_main_power = light_pwm_duty > 0;

        let auto_outputs = AutoControlOutputs {
            ventilation_fan,
            target_fan_speed_rpm,
            humidifier,
            water_pump,
            light_main_power,
            light_pwm_duty,
        };

        if auto_outputs != last_auto_outputs {
            info!(
                "AUTO dispatcher -> fan={} rpm={} humidifier={} pump={} light={} duty={}",
                auto_outputs.ventilation_fan,
                auto_outputs.target_fan_speed_rpm,
                auto_outputs.humidifier,
                auto_outputs.water_pump,
                auto_outputs.light_main_power,
                auto_outputs.light_pwm_duty
            );
        }

        {
            let mut state = GLOBAL_STATE.lock().await;
            state.target.ventilation_fan = Some(auto_outputs.ventilation_fan);
            state.target.target_fan_speed_rpm = Some(auto_outputs.target_fan_speed_rpm);
            state.target.humidifier = Some(auto_outputs.humidifier);
            state.target.water_pump = Some(auto_outputs.water_pump);
            state.target.light_main_power = Some(auto_outputs.light_main_power);
            state.target.light_pwm_duty = Some(auto_outputs.light_pwm_duty);
        }

        last_auto_outputs = auto_outputs;
        ticker.next().await;
    }
}

fn hysteresis_high_side(current: f32, target: f32, on_margin: f32, off_margin: f32, prev_state: bool) -> bool {
    if prev_state {
        current > (target + off_margin)
    } else {
        current > (target + on_margin)
    }
}

fn hysteresis_low_side(current: f32, target: f32, on_margin: f32, off_margin: f32, prev_state: bool) -> bool {
    if prev_state {
        current < (target - off_margin)
    } else {
        current < (target - on_margin)
    }
}

fn compute_fan_speed_rpm(
    temperature: Option<f32>,
    target_temperature: Option<f32>,
    humidity_air: Option<f32>,
    target_humidity_air: Option<f32>,
) -> u32 {
    let temp_ratio = match (temperature, target_temperature) {
        (Some(current), Some(target)) => ((current - target) / 8.0).clamp(0.0, 1.0),
        _ => 0.0,
    };

    let humidity_ratio = match (humidity_air, target_humidity_air) {
        (Some(current), Some(target)) => ((current - target) / 30.0).clamp(0.0, 1.0),
        _ => 0.0,
    };

    let demand_ratio = temp_ratio.max(humidity_ratio);
    let span = (AUTO_FAN_MAX_RPM - AUTO_FAN_MIN_RPM) as f32;

    (AUTO_FAN_MIN_RPM as f32 + span * demand_ratio) as u32
}

fn compute_light_pwm_duty(
    light_intensity: Option<f32>,
    target_light_intensity: Option<f32>,
    prev_duty: u32,
) -> u32 {
    let desired_duty = match (light_intensity, target_light_intensity) {
        (_, Some(target)) if target <= 0.0 => 0,
        (Some(current), Some(target)) => {
            let deficit = target - current;

            if deficit <= -LIGHT_OFF_MARGIN_LUX {
                0
            } else if deficit <= 0.0 {
                // Already around the target: dim down gradually instead of hard-off.
                0
            } else {
                let proportional = ((deficit / target) * 100.0) as u32;
                proportional.clamp(LIGHT_PWM_MIN_ACTIVE_DUTY, 100)
            }
        }
        _ => 0,
    };

    if desired_duty > prev_duty {
        prev_duty.saturating_add(LIGHT_PWM_STEP_UP).min(desired_duty)
    } else {
        prev_duty.saturating_sub(LIGHT_PWM_STEP_DOWN).max(desired_duty)
    }
}
