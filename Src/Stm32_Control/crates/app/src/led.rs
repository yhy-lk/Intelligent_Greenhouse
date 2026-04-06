
use crate::*;
 
 // --- Task 1: Blinky ---
 // 只要传入的东西实现了 BoardLed，这个任务就能跑

pub async fn led_toggle<L>(led: &mut L)
where
    L: BoardLed,
{
    led.toggle();
}