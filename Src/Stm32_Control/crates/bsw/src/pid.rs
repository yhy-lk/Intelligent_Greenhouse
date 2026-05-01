// #![no_std]

/// 离散位置式 PID 控制器
pub struct PidController {
    integral: f32,
    prev_error: f32,
    integral_limit: f32,
    output_limit: f32,
    last_error: f32,
    last_derivative: f32,
    last_output: f32,
}

impl PidController {
    pub fn new(integral_limit: f32, output_limit: f32) -> Self {
        Self {
            integral: 0.0,
            prev_error: 0.0,
            integral_limit,
            output_limit,
            last_error: 0.0,
            last_derivative: 0.0,
            last_output: 0.0,
        }
    }

    pub fn reset(&mut self) {
        self.integral = 0.0;
        self.prev_error = 0.0;
        self.last_error = 0.0;
        self.last_derivative = 0.0;
        self.last_output = 0.0;
    }

    pub fn compute(
        &mut self,
        setpoint: f32,
        measured: f32,
        dt: f32,
        kp: f32,
        ki: f32,
        kd: f32,
    ) -> f32 {
        let error = setpoint - measured;

        // 积分累加与原生极值限幅 (Anti-windup)
        self.integral = (self.integral + error * dt).clamp(-self.integral_limit, self.integral_limit);

        // 微分计算 (防 dt 为 0 导致除零错误)
        let derivative = if dt > 0.0001 {
            (error - self.prev_error) / dt
        } else {
            0.0
        };

        // 计算输出总和并使用原生极值限幅
        let output = (kp * error) + (ki * self.integral) + (kd * derivative);
        let constrained_output = output.clamp(-self.output_limit, self.output_limit);

        self.last_error = error;
        self.last_derivative = derivative;
        self.last_output = constrained_output;
        self.prev_error = error;
        constrained_output
    }

    pub fn integral(&self) -> f32 {
        self.integral
    }

    pub fn integral_limit(&self) -> f32 {
        self.integral_limit
    }

    pub fn output_limit(&self) -> f32 {
        self.output_limit
    }

    pub fn last_error(&self) -> f32 {
        self.last_error
    }

    pub fn last_derivative(&self) -> f32 {
        self.last_derivative
    }

    pub fn last_output(&self) -> f32 {
        self.last_output
    }
}
