// #![no_std]

use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_time::{Duration, Timer};

/// 硬件执行器自动化冒烟测试任务
/// 测试流程：水泵 -> 加湿器 -> 遮阳棚 -> 补风电机 -> WS2812 补光灯
#[embassy_executor::task]
pub async fn hardware_smoke_test_task() {
    info!("Hardware smoke test will start in 5 seconds...");
    Timer::after(Duration::from_secs(5)).await;

    // ==========================================
    // 1. 测试水泵 (继电器/MOS管)
    // ==========================================
    info!(">>> TEST 1: Water Pump ON");
    { GLOBAL_STATE.lock().await.target.water_pump = Some(true); }
    Timer::after(Duration::from_secs(3)).await;
    
    { GLOBAL_STATE.lock().await.target.water_pump = Some(false); }
    info!(">>> TEST 1: Water Pump OFF");
    Timer::after(Duration::from_secs(2)).await;

    // ==========================================
    // 2. 测试加湿器 (继电器/MOS管)
    // ==========================================
    info!(">>> TEST 2: Humidifier ON");
    { GLOBAL_STATE.lock().await.target.humidifier = Some(true); }
    Timer::after(Duration::from_secs(3)).await;
    
    { GLOBAL_STATE.lock().await.target.humidifier = Some(false); }
    info!(">>> TEST 2: Humidifier OFF");
    Timer::after(Duration::from_secs(2)).await;

    // ==========================================
    // 3. 测试遮阳棚电机 (简单高低电平开关/使能)
    // ==========================================
    info!(">>> TEST 3: Sunshade Motor ON");
    { GLOBAL_STATE.lock().await.target.window_actuator = Some(true); }
    Timer::after(Duration::from_secs(3)).await;
    
    { GLOBAL_STATE.lock().await.target.window_actuator = Some(false); }
    info!(">>> TEST 3: Sunshade Motor OFF");
    Timer::after(Duration::from_secs(2)).await;

    // ==========================================
    // 4. 测试补风电机 (PID 闭环与 PWM)
    // ==========================================
    info!(">>> TEST 4: Ventilation Fan ENABLED, Target: 2000 RPM");
    {
        let mut state = GLOBAL_STATE.lock().await;
        state.target.ventilation_fan = Some(true);
        state.target.target_fan_speed_rpm = Some(2000);
    }
    Timer::after(Duration::from_secs(4)).await; // 给予足够时间让 PID 稳定

    info!(">>> TEST 4: Ventilation Fan Target: 5000 RPM");
    { GLOBAL_STATE.lock().await.target.target_fan_speed_rpm = Some(5000); }
    Timer::after(Duration::from_secs(4)).await;
    
    { GLOBAL_STATE.lock().await.target.ventilation_fan = Some(false); }
    info!(">>> TEST 4: Ventilation Fan OFF");
    Timer::after(Duration::from_secs(2)).await;

    // ==========================================
    // 5. 测试 WS2812 补光灯 (颜色与亮度解析)
    // ==========================================
    info!(">>> TEST 5: WS2812 Light ENABLED (RED)");
    {
        let mut state = GLOBAL_STATE.lock().await;
        state.target.light_main_power = Some(true);
        state.target.light_pwm_duty = Some(50); // 50% 亮度防刺眼
        state.target.light_color_rgb = Some(0x00FF0000); // 纯红
    }
    Timer::after(Duration::from_secs(2)).await;

    info!(">>> TEST 5: WS2812 Light (GREEN)");
    { GLOBAL_STATE.lock().await.target.light_color_rgb = Some(0x0000FF00); } // 纯绿
    Timer::after(Duration::from_secs(2)).await;

    info!(">>> TEST 5: WS2812 Light (BLUE)");
    { GLOBAL_STATE.lock().await.target.light_color_rgb = Some(0x000000FF); } // 纯蓝
    Timer::after(Duration::from_secs(2)).await;

    { GLOBAL_STATE.lock().await.target.light_main_power = Some(false); }
    info!(">>> TEST 5: WS2812 Light OFF");

    // ==========================================
    // 测试结束
    // ==========================================
    info!("=== All Hardware Tests Completed ===");
    
    // 挂起该任务，不占用 CPU
    core::future::pending::<()>().await;
}