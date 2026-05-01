// #![no_std]

use bsw::can_proto::{build_tx_frame, FuncCode};
use bsw::protocol::ParamIndex;
use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_stm32::can::CanTx;
use embassy_time::{Duration, Ticker};

/// CAN 遥测上报任务
#[embassy_executor::task]
pub async fn can_tx_task(mut tx: CanTx<'static>) {
    info!("Starting CAN TX task (1Hz telemetry)");
    let mut ticker = Ticker::every(Duration::from_millis(1000));

    loop {
        ticker.next().await;

        // 1. 提取微秒级数据快照
        let snap = { GLOBAL_STATE.lock().await.current };

        // 2. 优先处理高优先级警报 (Alert 0x0)
        if let Some(err_code) = snap.error_code {
            if err_code != 0 {
                let frame = build_tx_frame(FuncCode::Alert, ParamIndex::ErrorCode as u8, err_code);
                let _ = tx.write(&frame).await;
            }
        }

        // ==========================================
        // 3. 表驱动发送：处理模拟量 (f32 -> u32)
        // ==========================================
        let analog_metrics = [
            (ParamIndex::Temperature, snap.temperature),
            (ParamIndex::HumidityAir, snap.humidity_air),
            (ParamIndex::HumiditySoil, snap.humidity_soil),
            (ParamIndex::LightIntensity, snap.light_intensity), 
            (ParamIndex::SoilPh, snap.soil_ph),
        ];

        for (index, opt_val) in analog_metrics {
            if let Some(val_f32) = opt_val {
                // 调用协议层映射表进行缩放转换
                let val_u32 = index.scale_f32_to_u32(val_f32);
                let frame = build_tx_frame(FuncCode::Report, index as u8, val_u32);
                let _ = tx.write(&frame).await;
            }
        }

        // ==========================================
        // 4. 表驱动发送：处理整型量 (直接 u32 发送)
        // ==========================================
        let digital_metrics = [
            (ParamIndex::Co2Level, snap.co2_level),
            (ParamIndex::FanSpeed, snap.fan_speed_rpm),
            (ParamIndex::SoilEc, snap.soil_ec),
            (ParamIndex::WaterLevel, snap.water_level),
            (ParamIndex::Heartbeat, snap.system_uptime),
        ];

        for (index, opt_val) in digital_metrics {
            if let Some(val_u32) = opt_val {
                let frame = build_tx_frame(FuncCode::Report, index as u8, val_u32);
                let _ = tx.write(&frame).await;
            }
        }

        debug!("CAN TX: Telementry cycle completed");
    }
}