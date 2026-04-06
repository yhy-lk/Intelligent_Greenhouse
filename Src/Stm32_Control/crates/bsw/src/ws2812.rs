use embassy_stm32::timer::simple_pwm::{SimplePwm};
use embassy_stm32::timer::{GeneralInstance4Channel, Channel, UpDma};
use embassy_time::Timer;
use smart_leds_trait::{RGB8};
use embassy_stm32::Peri;


// 定义灯珠数量
const LED_COUNT: usize = 25; 
// 定义 DMA Buffer 大小 (24 bit/led + 40 bit reset)
const DMA_BUF_SIZE: usize = LED_COUNT * 24 + 40;


pub struct Ws2812<'d, T, DMA>
where
    T: GeneralInstance4Channel,
    DMA: UpDma<T>,
{
    pwm: SimplePwm<'d, T>,
    dma: Peri<'d, DMA>,
    channel: Channel,
    // 预计算的占空比
    duty0: u16,
    duty1: u16,
    // 内部持有的 DMA Buffer，为了方便，这里假设最大支持数量
    // 如果想要极致省内存，可以把这个 buffer 放在外面传入，但作为 Driver，放在里面更易用
    // 假设最大 100 颗灯，需要内存：100 * 24 * 2 = 4.8KB，F407 内存很大，随便霍霍
    buffer: [u16; DMA_BUF_SIZE], 
}

impl<'d, T, DMA> Ws2812<'d, T, DMA>
where
    T: GeneralInstance4Channel,
    DMA: UpDma<T>,
{
    pub fn new(pwm: SimplePwm<'d, T>, dma: Peri<'d, DMA>, channel: Channel) -> Self {
        // pwm.enable(channel);
        let max = pwm.max_duty_cycle() as u16;
        Self {
            pwm,
            dma,
            channel,
            duty0: max * 8 / 25, // 32%
            duty1: max * 2 / 3,  // 66%
            buffer: [0; DMA_BUF_SIZE],
        }
    }
}

impl<'d, T, DMA> Ws2812<'d, T, DMA>
where
    T: GeneralInstance4Channel,
    DMA: UpDma<T>,
{
    // 这才是你真正调用的方法
    pub async fn write<I>(&mut self, iterator: I) 
    where
        I: IntoIterator<Item = RGB8>,
    {
        let mut cursor = 0;
        for color in iterator {

            if cursor >= LED_COUNT * 24 {
                break; // 防止溢出
            }

            let bits = [color.g, color.r, color.b];
            for byte in bits {
                for i in (0..8).rev() {
                    let is_high = (byte >> i) & 1 == 1;
                    self.buffer[cursor] = if is_high { self.duty1 } else { self.duty0 };
                    cursor += 1;
                }
            }
        }
        
        // 补齐 Reset 信号
        let len = cursor + 40;
        if len <= self.buffer.len() {
            self.buffer[cursor..len].fill(0);
        }

        // 发送
        self.pwm.waveform_up(self.dma.reborrow(), self.channel, &self.buffer[..len]).await;

        Timer::after_micros(50).await;
    }
}