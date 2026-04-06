use crate::shared::BlockingMasterI2cDevice;
use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_time::{Duration, Ticker, Timer};

// 连续失败阈值，超过则判定传感器离线
const MAX_FAILURES: u8 = 3;

#[embassy_executor::task]
pub async fn bh1750_task(
    i2c: BlockingMasterI2cDevice,
) {
    trace!("Starting BH1750 task");
    let mut bh1750 = bsw::bh1750::Bh1750::new(i2c, bsw::bh1750::Bh1750Address::Addr23);

    // 1. 初始化重试机制
    loop {
        // 假设 init 是异步函数
        match app::bh1750::init(&mut bh1750).await {
            Ok(_) => {
                info!("BH1750 initialized successfully");
                break;
            }
            Err(e) => {
                error!("BH1750 init failed: {:?}, retrying in 2s...", e);
                Timer::after(Duration::from_secs(2)).await;
            }
        }
    }

    let mut ticker = Ticker::every(Duration::from_secs(1));
    // 显式指定类型解决类型推断歧义
    let mut fail_count: u8 = 0;

    loop {
        ticker.next().await;
        trace!("Reading BH1750 measurement...");

        // 执行传感器读取（耗时操作），此时不持有锁
        // 假设 read 是同步函数；若是异步则必须加 .await
        let measurement_result = app::bh1750::read(&mut bh1750);

        match measurement_result {
            Ok(meas) => {
                fail_count = 0;
                
                // 开启局部作用域，精确控制锁的生命周期
                {
                    let mut state = GLOBAL_STATE.lock().await;
                    state.current.light_intensity = Some(meas);
                } // state 离开作用域，锁在此处立即释放
           
                trace!("BH1750 Measurement - Lux: {}", meas);
            }
            Err(e) => {
                error!("BH1750 read failed: {:?}", e);
                fail_count = fail_count.saturating_add(1);

                if fail_count >= MAX_FAILURES {
                    // 同样使用局部作用域写入错误状态
                    {
                        let mut state = GLOBAL_STATE.lock().await;
                        state.current.light_intensity = None;
                    }
                    warn!("BH1750 marked as offline due to consecutive failures");
                    
                    // 为防止计数器溢出或无限累加，可将计数器维持在阈值
                    fail_count = MAX_FAILURES;
                }
            }
        }
    }
}