use embassy_stm32::timer::{GeneralInstance4Channel, UpDma};
// use embassy_stm32::timer::simple_pwm::{SimplePwm};
// use embassy_stm32::timer::{GeneralInstance4Channel, Channel, UpDma};
use smart_leds::{hsv::{hsv2rgb, Hsv}};
use smart_leds_trait::RGB8;

use bsw::ws2812::Ws2812;
// use service::colors;

pub async fn all_on_hsv<'a, T, DMA>(
    ws2812:&mut Ws2812<'a, T, DMA>, 
    led_count: usize,
    hsv: Hsv
) 
where 
    T: GeneralInstance4Channel,
    DMA: UpDma<T>, 
{
    let all_on_iterator = (0..led_count).map(|_| {
        hsv2rgb(hsv) // 全白
    });
    ws2812.write(all_on_iterator).await;
}

pub async fn all_on_rgb8<'a, T, DMA>(
    ws2812: &mut Ws2812<'a, T, DMA>, 
    led_count: usize,
    rgb8: RGB8
) 
where 
    T: GeneralInstance4Channel,
    DMA: UpDma<T>, 
{
    let all_on_iterator = (0..led_count).map(|_| {
        rgb8 // 全白
    });
    ws2812.write(all_on_iterator).await;
}

pub async fn fade_on<'a, T, DMA>(
    ws2812:&mut Ws2812<'a, T, DMA>, 
    led_count: usize, 
    step: usize,
    hsv: Hsv
)
where 
    T: GeneralInstance4Channel,
    DMA: UpDma<T>, 
{
    const TRAIL_LEN: usize = 8; // 流星尾巴长度
    let meteor_iterator = (0..led_count).map(|i| {
            // 1. 计算当前灯珠 i 距离 "头(step)" 的距离
            //    这里涉及到一个环形缓冲区的算法：
            //    如果 step 在 5，i 在 4，距离是 1。
            //    如果 step 在 0，i 在 15 (总数16)，距离也是 1 (跨过终点回绕)。
            
            // 这里的数学公式：(当前步数 - 灯珠索引 + 总灯数) % 总灯数
            // 结果 0 代表是头，1 代表尾巴第1节，2 代表尾巴第2节...
            let distance = (step + led_count - i) % led_count;

            // 2. 判断是否在拖尾范围内
            if distance < TRAIL_LEN as usize {
                // 3. 计算亮度 (Value)
                //    距离越近(distance小) -> 亮度越高
                //    线性衰减公式：
                let decay = 255 / TRAIL_LEN as u16; // 每一节亮度减多少
                let val = 255 - (distance as u16 * decay);
                
                // 4. 生成颜色
                //    这里我弄了个青色 (Cyan, Hue=128)，你可以改成动态的
                hsv2rgb(Hsv {
                    hue: hsv.hue,          // 固定青色，或者改成 step as u8 让流星变色
                    sat: hsv.sat,          // 饱和度拉满
                    val: val as u8,    // <--- 亮度由距离决定
                })
            } else {
                // 不在拖尾范围内，关灯
                RGB8::default() 
            }
        });
        ws2812.write(meteor_iterator).await;
    
}

pub async fn rainbow<'a, T, DMA>(
    ws2812:&mut Ws2812<'a, T, DMA>, 
    led_count: usize, 
    step: usize
)
where 
    T: GeneralInstance4Channel,
    DMA: UpDma<T>, 
{
    let rainbow_iterator = (0..led_count).map(|i| {
        // 2. 这里的闭包 (Closure) 就是你的"算法"
        //    i: 当前是第几个灯
        //    step: 当前时间/相位
        
        // 计算当前灯的色相 (Hue)
        // 比如 step=0 时，第0个灯hue=0(红)，第10个灯hue=10(橙)
        // wrapping_add 防止 u8 溢出 panic
        let hue = (step.wrapping_add(i * 20)) as u8;
        
        // 构造 HSV 颜色
        let hsv = Hsv {
            hue,
            sat: 255, // 饱和度拉满
            val: 30,  // 亮度调低点 (保护眼睛！)
        };
        
        // 3. 转成 RGB8
        hsv2rgb(hsv)
    });
    ws2812.write(rainbow_iterator).await;
    
}

