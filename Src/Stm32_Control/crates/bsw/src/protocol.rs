

// ==========================================
// 1. 通信协议参数与状态枚举
// ==========================================
#[repr(u8)]
#[derive(Copy, Clone, Eq, PartialEq, Debug)]
pub enum ParamIndex {
    // A. 系统级指令 (System)
    SystemTimestamp = 0x00,
    Heartbeat       = 0x01,
    ErrorCode       = 0x02,
    ControlMode     = 0x03,

    // B. 开关/状态量 (Switches / Boolean)
    LightMainPower  = 0x10,
    WaterPump       = 0x11,
    Humidifier      = 0x12,
    VentilationFan  = 0x13,
    SunshadeMotor   = 0x14,
    Heater          = 0x15,
    WindowActuator  = 0x16,
    AuxRelay1       = 0x17,
    AuxRelay2       = 0x18,

    // C. 模拟量：环境值与设定 (Analog Values)
    Temperature     = 0x30,
    HumidityAir     = 0x31,
    HumiditySoil    = 0x32,
    LightIntensity  = 0x33,
    Co2Level        = 0x34,
    FanSpeed        = 0x35,
    SoilPh          = 0x36,
    SoilEc          = 0x37,
    WaterLevel      = 0x38,

    // D. 复杂参数 (Advanced)
    LightColor      = 0x50,
    LightPwmDuty    = 0x51,
    PidP            = 0x52,
    PidI            = 0x53,
    PidD            = 0x54,
}

/// 系统控制模式枚举
#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub enum ControlMode {
    Manual,
    Auto,
}

// ==========================================
// 2. 内部物理状态结构体 (Physical State)
// ==========================================

/// 当前传感器与系统读取值
/// 全部使用 Option 封装，None 表示尚未读取到有效数据
#[derive(Default, Clone, Copy, Debug)]
pub struct CurrentValues {
    // 系统状态
    pub system_uptime: Option<u32>,       // 秒
    pub error_code: Option<u32>,          // 错误掩码

    // 环境模拟量 (真实物理单位)
    pub temperature: Option<f32>,         // ℃
    pub humidity_air: Option<f32>,        // %RH
    pub humidity_soil: Option<f32>,       // %RH
    pub light_intensity: Option<f32>,     // Lux
    pub co2_level: Option<u32>,           // ppm
    pub fan_speed_rpm: Option<u32>,       // RPM
    pub soil_ph: Option<f32>,             // pH
    pub soil_ec: Option<u32>,             // us/cm
    pub water_level: Option<u32>,         // %
}

/// 执行器状态与设定目标值
/// 全部使用 Option 封装，None 表示主控尚未下发或未初始化设定值
#[derive(Default, Clone, Copy, Debug)]
pub struct TargetValues {
    // 系统设定
    pub control_mode: Option<ControlMode>,
    pub system_timestamp: Option<u32>,

    // 开关状态 (false: OFF/收起/关窗, true: ON/展开/开窗)
    pub light_main_power: Option<bool>,
    pub water_pump: Option<bool>,
    pub humidifier: Option<bool>,
    pub ventilation_fan: Option<bool>,
    pub sunshade_motor: Option<bool>,
    pub heater: Option<bool>,
    pub window_actuator: Option<bool>,
    pub aux_relay_1: Option<bool>,
    pub aux_relay_2: Option<bool>,

    // 目标模拟量 (真实物理单位)
    pub target_temperature: Option<f32>,
    pub target_humidity_air: Option<f32>,
    pub target_humidity_soil: Option<f32>,
    pub target_light_intensity: Option<f32>,
    pub target_co2_level: Option<u32>,
    pub target_fan_speed_rpm: Option<u32>,
    pub target_soil_ph: Option<f32>,
    pub target_soil_ec: Option<u32>,
    pub target_water_level: Option<u32>,

    // 复杂参数
    pub light_color_rgb: Option<u32>,
    pub light_pwm_duty: Option<u32>,
    pub pid_p: Option<f32>,
    pub pid_i: Option<f32>,
    pub pid_d: Option<f32>,
}

pub struct SystemState {
    pub current: CurrentValues,
    pub target: TargetValues,
}

// ==========================================
// 3. 全局单例 (Global Singleton)
// ==========================================

/// 全局状态互斥锁。
/// 依赖于 Option 的 Default 实现，所有变量初始状态隐式为 None。

impl CurrentValues {
    /// 供 const 上下文使用的默认值初始化器
    pub const fn default_impl() -> Self {
        Self {
            system_uptime: None,
            error_code: None,
            temperature: None,
            humidity_air: None,
            humidity_soil: None,
            light_intensity: None,
            co2_level: None,
            fan_speed_rpm: None,
            soil_ph: None,
            soil_ec: None,
            water_level: None,
        }
    }
}

impl TargetValues {
    /// 供 const 上下文使用的默认值初始化器
    pub const fn default_impl() -> Self {
        Self {
            // 控制模式建议给予一个确定的初始值，或在初始化阶段显式配置
            control_mode: Some(ControlMode::Auto),
            system_timestamp: None,
            light_main_power: None,
            water_pump: None,
            humidifier: None,
            ventilation_fan: None,
            sunshade_motor: None,
            heater: None,
            window_actuator: None,
            aux_relay_1: None,
            aux_relay_2: None,
            target_temperature: None,
            target_humidity_air: None,
            target_humidity_soil: None,
            target_light_intensity: None,
            target_co2_level: None,
            target_fan_speed_rpm: None,
            target_soil_ph: None,
            target_soil_ec: None,
            target_water_level: None,
            light_color_rgb: None,
            light_pwm_duty: None,
            pid_p: None,
            pid_i: None,
            pid_d: None,
        }
    }
}


impl ParamIndex {
    /// 协议缩放表：将内部真实的 f32 物理量转换为 CAN 总线传输的 u32
    pub fn scale_f32_to_u32(self, physical_val: f32) -> u32 {
        let factor = match self {
            // 需要放大 100 倍的参数
            Self::Temperature | 
            Self::HumidityAir | 
            Self::HumiditySoil | 
            Self::SoilPh => 100.0,
            
            // 默认缩放因子为 1.0 (光照等)
            _ => 1.0, 
        };
        (physical_val * factor) as u32
    }
}