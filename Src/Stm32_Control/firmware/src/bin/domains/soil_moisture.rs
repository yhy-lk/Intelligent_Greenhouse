use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_stm32::adc::{Adc, SampleTime};
use embassy_time::{Duration, Timer};

#[embassy_executor::task]
pub async fn soil_moisture_task(
    mut adc: Adc<'static, embassy_stm32::peripherals::ADC1>, 
    mut pc1: embassy_stm32::Peri<'static, embassy_stm32::peripherals::PC1>,
) {
    trace!("Starting Soil Moisture ADC task");
    
    let mut vrefint_channel = adc.enable_vrefint();
    let mut temp_channel = adc.enable_temperature();

    loop {
        // ADC 转换在 STM32F4 上通常仅需几微秒，blocking_read 不会对异步调度器造成实质性阻塞
        let humidity_raw = adc.blocking_read(&mut pc1, SampleTime::CYCLES480) as f32;
        let vrefint_raw = adc.blocking_read(&mut vrefint_channel, SampleTime::CYCLES480) as f32;
        let temp_raw = adc.blocking_read(&mut temp_channel, SampleTime::CYCLES480) as f32;
        
        // --- 核心转换逻辑 ---

        // 1. 计算系统真实供电电压 (VDDA)
        let vdda_voltage = (1.21 * 4095.0) / vrefint_raw;

        // 2. 计算芯片内部温度 (摄氏度)
        let v_sense = (temp_raw / 4095.0) * vdda_voltage;
        let temperature_c = ((v_sense - 0.76) / 0.0025) + 25.0;

        // 3. 计算土壤湿度百分比 (0% ~ 100%)
        let raw_mapped = (4000.0 - humidity_raw) / (4000.0 - 1000.0) * 100.0;
        
        // 优化：使用标准库的 clamp 方法替代冗长的 if-else 边界判断
        let humidity_percent = raw_mapped.clamp(0.0, 100.0);

        // 4. 更新全局单例状态 (仅暴露土壤湿度)
        {
            let mut state = GLOBAL_STATE.lock().await;
            state.current.humidity_soil = Some(humidity_percent);
        }

        // --- 打印直观数据 ---
        trace!(
            "实时数据报告:\n \
             - 土壤湿度: {}%\n \
             - 芯片温度: {} °C (内部)\n \
             - 系统供电: {} V (内部)\n\
             ------------------------",
            humidity_percent, temperature_c, vdda_voltage
        );

        // 异步休眠 1 秒
        Timer::after(Duration::from_millis(1000)).await;
    }
}