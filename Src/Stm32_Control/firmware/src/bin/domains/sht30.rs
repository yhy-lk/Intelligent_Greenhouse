use crate::shared::BlockingMasterI2cDevice;
use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_time::{Duration, Ticker, Timer};

// 连续失败阈值，超过则判定传感器离线
const MAX_FAILURES: u8 = 3;

#[embassy_executor::task]
pub async fn sht30_task(
    i2c: BlockingMasterI2cDevice,
) {
    trace!("Starting SHT30 task");
    let mut sht30 = bsw::sht30::Sht30::new(i2c, bsw::sht30::Sht30Address::Addr44);
    
    // 1. 初始化无限重试机制
    loop {
        match app::sht30::init(&mut sht30).await {
            Ok(_) => {
                info!("SHT30 initialized successfully");
                break;
            }
            Err(e) => {
                error!("SHT30 init failed: {:?}, retrying in 2s...", e);
                Timer::after(Duration::from_secs(2)).await;
            }
        }
    }

    let mut ticker = Ticker::every(Duration::from_secs(1));
    let mut fail_count: u8 = 0;

    loop {
        ticker.next().await;
        trace!("Reading SHT30 measurement...");

        // 提取底层的 Result，保留原始错误信息进行匹配
        let measurement_result = app::sht30::read(&mut sht30).await;

        match measurement_result {
            Ok(meas) => {
                fail_count = 0;
                
                // 开启局部作用域，快速获取并释放 Mutex 锁
                {
                    let mut state = GLOBAL_STATE.lock().await;
                    state.current.temperature = Some(meas.temp_c);
                    state.current.humidity_air = Some(meas.hum_rh);
                } 
                
                info!("SHT30 Measurement - Temp: {} °C, Humidity: {} %RH", meas.temp_c, meas.hum_rh);
            }
            Err(e) => {
                error!("SHT30 read failed: {:?}", e);
                fail_count = fail_count.saturating_add(1);

                if fail_count >= MAX_FAILURES {
                    // 连续失效，向全局状态广播 None
                    {
                        let mut state = GLOBAL_STATE.lock().await;
                        state.current.temperature = None;
                        state.current.humidity_air = None;
                    }
                    warn!("SHT30 marked as offline due to consecutive failures");
                    
                    fail_count = MAX_FAILURES;
                }
            }
        }
    }
}