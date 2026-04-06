use crate::shared::{Ws2812, GLOBAL_STATE};
use smart_leds::RGB8;
use embassy_time::{Duration, Ticker};

#[embassy_executor::task]
pub async fn fill_light_task(
    mut ws2812: Ws2812,
) {
    const LED_COUNT: usize = 25;
    let mut ticker = Ticker::every(Duration::from_millis(50));
    let mut step: usize = 0;

    // 状态追踪：避免数据未变化时持续触发 DMA 搬运占用总线带宽
    let mut current_output: Option<RGB8> = None;

    loop {
        // 1. 获取全局单例中的目标状态
        // 提取 开关状态、颜色设定、占空比(亮度)设定，并赋予默认防错值
        let (power, color_raw, duty) = {
            let state = GLOBAL_STATE.lock().await;
            (
                state.target.light_main_power.unwrap_or(false),
                state.target.light_color_rgb.unwrap_or(0x00FFFFFF), // 默认白光
                state.target.light_pwm_duty.unwrap_or(100).clamp(0, 100), // 限制在 0-100%
            )
        };

        // 2. 物理量推导与计算
        let target_rgb = if power {
            // 从 0x00RRGGBB 掩码中解构分离出独立通道数值
            let r_raw = ((color_raw >> 16) & 0xFF) as u32;
            let g_raw = ((color_raw >> 8) & 0xFF) as u32;
            let b_raw = (color_raw & 0xFF) as u32;

            // 亮度映射：通过占比相乘实现等比线性调光
            RGB8::new(
                ((r_raw * duty) / 100) as u8,
                ((g_raw * duty) / 100) as u8,
                ((b_raw * duty) / 100) as u8,
            )
        } else {
            // 电源关闭标志：全通道归零
            RGB8::new(0, 0, 0)
        };

        // 3. 执行变更评估与输出
        // 客观分析：若后续解除 `rainbow` 等动态刷新动画的注释，由于每一帧的颜色都在变化，
        // 此处的 `if` 拦截机制将自动失去抑制作用并退化为持续输出，不影响动画逻辑。
        if Some(target_rgb) != current_output {
            app::ws2812::all_on_rgb8(&mut ws2812, LED_COUNT, target_rgb).await;
            current_output = Some(target_rgb);
        }

        step = step.wrapping_add(1);
        ticker.next().await;
    }
}
