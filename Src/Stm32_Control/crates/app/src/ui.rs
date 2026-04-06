

use crate::*;

// 只要传入的东西实现了 Screen (DrawTarget)，这个任务就能跑
pub async fn draw_ui<D, I>(display: &mut D, led_toggle_count: I)
where
    D: DrawTarget<Color = BinaryColor, Error = DisplayError>,
    I: core::fmt::Display,
{

    let text_style = MonoTextStyleBuilder::new()
        .font(&FONT_6X10)
        .text_color(BinaryColor::On)
        .build();

    display.clear(BinaryColor::Off).unwrap();

    Text::with_baseline(
        "STM32F4 Embassy",
        Point::new(0, 0),
        text_style,
        Baseline::Top,
    )
    .draw(display)
    .unwrap();

    Text::with_baseline(
        "Mode: DMA Async",
        Point::new(0, 16),
        text_style,
        Baseline::Top,
    )
    .draw(display)
    .unwrap();

    let mut buf = String::<32>::new();

    write!(buf, "Count: {}", led_toggle_count).unwrap();

    Text::with_baseline(&buf, Point::new(0, 32), text_style, Baseline::Top)
        .draw(display)
        .unwrap();

}
