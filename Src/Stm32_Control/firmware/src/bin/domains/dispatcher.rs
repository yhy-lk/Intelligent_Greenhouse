
use crate::shared::Ws2812Sender;
use embassy_time::Ticker;
// use smart_leds::RGB8;
use smart_leds::{hsv::{hsv2rgb, Hsv}};

#[embassy_executor::task]
pub async fn dispatcher_task(
    fill_light_sender: Ws2812Sender,
) {
    let mut hsv = Hsv { hue: 0, sat: 255, val: 10 }; // 红色
    fill_light_sender.send(hsv2rgb(hsv)).await;
    let mut ticker = Ticker::every(embassy_time::Duration::from_millis(10));
    loop {
        hsv.hue = (hsv.hue + 1) as u8; // 每次增加1度，形成彩虹效果
        fill_light_sender.send(hsv2rgb(hsv) ).await;

        ticker.next().await;
        // 这里可以添加更多的调度逻辑，例如根据系统状态发送不同的颜色
        // 例如：如果温度过高，发送红色；如果湿度过低，发送蓝色；等等。
    }

}