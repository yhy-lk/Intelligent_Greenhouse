// #![no_std]

use crate::pid::PidController;
use embassy_stm32::gpio::Output;
use embassy_stm32::peripherals::{TIM1, TIM3};
use embassy_stm32::timer::qei::Qei;
use embassy_stm32::timer::simple_pwm::SimplePwm;

// ==========================================
// 物理模型常量定标
// ==========================================
const ENCODER_TICKS_PER_REV: f32 = 64.0;
const ENCODER_DIRECTION_SIGN: f32 = -1.0;
const PID_INTEGRAL_LIMIT_SCALE: f32 = 5.0;
pub const WHEEL_RADIUS_M: f32 = 0.05;
const PI: f32 = core::f32::consts::PI;

pub struct FanMotor<'d> {
    pwm: SimplePwm<'d, TIM1>,
    dir_in1: Output<'d>,
    dir_in2: Output<'d>,
    qei: Qei<'d, TIM3>,
    pid: PidController,
    
    // 状态快照
    max_duty: u32,
    last_qei_count: u16, // 必须强制为 u16，以匹配 TIM3 硬件寄存器宽度
    current_rpm: f32,
    last_pwm_command: f32,
    last_pwm_duty: u32,
}

impl<'d> FanMotor<'d> {
    pub fn new(
        mut pwm: SimplePwm<'d, TIM1>,
        dir_in1: Output<'d>,
        dir_in2: Output<'d>,
        qei: Qei<'d, TIM3>,
    ) -> Self {
        let mut ch1 = pwm.ch1();
        ch1.enable();
        
        // 兼容新版 API，max_duty_cycle 返回 u32
        let max_duty = ch1.max_duty_cycle();

        Self {
            pwm,
            dir_in1,
            dir_in2,
            qei,
            pid: PidController::new((max_duty as f32) * PID_INTEGRAL_LIMIT_SCALE, max_duty as f32),
            max_duty,
            last_qei_count: 0,
            current_rpm: 0.0,
            last_pwm_command: 0.0,
            last_pwm_duty: 0,
        }
    }

    // ==========================================
    // 物理量测算逻辑
    // ==========================================

    pub fn sync_encoder(&mut self) {
        // 提取时强制转为 u16，剔除高 16 位的无效数据
        self.last_qei_count = self.qei.count() as u16;
    }

    fn measure_rpm(&mut self, dt_sec: f32) -> f32 {
        let current_count = self.qei.count() as u16;
        
        // 必须使用 16 位整数进行 wrapping_sub，随后转为 i16 获取正确的正负方向
        let delta_ticks = current_count.wrapping_sub(self.last_qei_count) as i16;
        self.last_qei_count = current_count;

        if dt_sec > 0.0 {
            // 将编码器计数方向对齐到“正 PWM = 正转速”的软件约定。
            self.current_rpm = ((delta_ticks as f32) * ENCODER_DIRECTION_SIGN / ENCODER_TICKS_PER_REV)
                * (60.0 / dt_sec);
        } else {
            self.current_rpm = 0.0;
        }
        self.current_rpm
    }

    pub fn get_linear_velocity(&self) -> f32 {
        (2.0 * PI * WHEEL_RADIUS_M * self.current_rpm) / 60.0
    }

    pub fn get_rpm(&self) -> f32 {
        self.current_rpm
    }

    pub fn get_max_duty(&self) -> u32 {
        self.max_duty
    }

    pub fn get_last_pwm_command(&self) -> f32 {
        self.last_pwm_command
    }

    pub fn get_last_pwm_duty(&self) -> u32 {
        self.last_pwm_duty
    }

    pub fn get_pid_error(&self) -> f32 {
        self.pid.last_error()
    }

    pub fn get_pid_integral(&self) -> f32 {
        self.pid.integral()
    }

    pub fn get_pid_integral_limit(&self) -> f32 {
        self.pid.integral_limit()
    }

    pub fn get_pid_derivative(&self) -> f32 {
        self.pid.last_derivative()
    }

    pub fn get_pid_output(&self) -> f32 {
        self.pid.last_output()
    }

    pub fn get_pid_output_limit(&self) -> f32 {
        self.pid.output_limit()
    }

    // ==========================================
    // 执行器闭环逻辑
    // ==========================================

    pub fn control_tick(&mut self, target_rpm: f32, dt_sec: f32, kp: f32, ki: f32, kd: f32) {
        if target_rpm == 0.0 {
            self.pid.reset();
            self.apply_hardware_pwm(0.0);
            self.measure_rpm(dt_sec); 
            return;
        }

        let actual_rpm = self.measure_rpm(dt_sec);
        let pwm_output = self.pid.compute(target_rpm, actual_rpm, dt_sec, kp, ki, kd);
        self.apply_hardware_pwm(pwm_output);
    }

    fn apply_hardware_pwm(&mut self, duty_signed: f32) {
        let forward = duty_signed >= 0.0;
        let duty_val = duty_signed.abs();

        // 占空比限幅，确保不溢出 ARR 寄存器
        let duty_u32 = (duty_val as u32).min(self.max_duty);
        self.last_pwm_command = duty_signed;
        self.last_pwm_duty = duty_u32;

        if duty_u32 == 0 {
            self.dir_in1.set_low();
            self.dir_in2.set_low();
        } else if forward {
            self.dir_in1.set_high();
            self.dir_in2.set_low();
        } else {
            self.dir_in1.set_low();
            self.dir_in2.set_high();
        }

        let mut ch1 = self.pwm.ch1();
        ch1.set_duty_cycle(duty_u32);
    }
}
