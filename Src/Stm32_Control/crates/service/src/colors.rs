use smart_leds::hsv::Hsv;

// 基础定义：默认全饱和度(255)，全亮度(255)
// 只有 Hue (色相) 是变化的
pub const RED:       Hsv = Hsv { hue: 0,   sat: 255, val: 255 };
pub const ORANGE:    Hsv = Hsv { hue: 25,  sat: 255, val: 255 }; // ~35度
pub const YELLOW:    Hsv = Hsv { hue: 43,  sat: 255, val: 255 }; // ~60度
pub const GREEN:     Hsv = Hsv { hue: 85,  sat: 255, val: 255 }; // 120度
pub const CYAN:      Hsv = Hsv { hue: 128, sat: 255, val: 255 }; // 180度
pub const BLUE:      Hsv = Hsv { hue: 170, sat: 255, val: 255 }; // 240度
pub const PURPLE:    Hsv = Hsv { hue: 213, sat: 255, val: 255 }; // 300度
pub const PINK:      Hsv = Hsv { hue: 235, sat: 255, val: 255 }; // ~330度

// 特殊颜色
pub const WHITE:     Hsv = Hsv { hue: 0,   sat: 0,   val: 255 }; // 饱和度为0就是白
pub const BLACK:     Hsv = Hsv { hue: 0,   sat: 0,   val: 0   }; // 亮度为0就是灭

/// 这是一个辅助函数：让开发者可以方便地调整预定义颜色的亮度
/// 比如：colors::with_brightness(colors::RED, 50); // 暗红色
pub const fn with_brightness(color: Hsv, brightness: u8) -> Hsv {
    Hsv {
        hue: color.hue,
        sat: color.sat,
        val: brightness,
    }
}