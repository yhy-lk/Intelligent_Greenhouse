use crate::shared::LedSingal;
use app;
use service::led::BoardLed;

#[embassy_executor::task]
pub async fn led_toggle_task(
    mut led: bsw::led::SystemLed<'static>,
    led_singal: &'static LedSingal,
) {
    let mut ticker = embassy_time::Ticker::every(embassy_time::Duration::from_millis(500));
    loop {
        app::led::led_toggle(&mut led).await;
        led_singal.signal(led.toggle_count());
        ticker.next().await;
    }
}
