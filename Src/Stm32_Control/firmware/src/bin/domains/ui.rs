use defmt::*;

// use embassy_sync::{
//     blocking_mutex::raw::{ThreadModeRawMutex},
//     signal::Signal,
// };
use {defmt_rtt as _, panic_probe as _};

use app;
use crate::shared::{ConcreteDisplay, LedSingal};


#[embassy_executor::task]
pub async fn oled_ui_task(
    mut display: ConcreteDisplay,
    led_toggle_count: &'static LedSingal,
) 

{
    let mut ticker = embassy_time::Ticker::every(embassy_time::Duration::from_millis(100));
    loop {
        let led_count = led_toggle_count.wait().await;

        app::ui::draw_ui(&mut display, led_count).await;

        trace!("Flushing display");
        display.flush()
            .await
            .inspect(|_| trace!("Display flush success"))
            .inspect_err(|_| error!("Display flush error"))
            .ok();

        // ticker.next().await;
        embassy_time::Timer::after_millis(100).await;
    }
}
