#![no_std]
// use embassy_sync::{blocking_mutex::raw::CriticalSectionRawMutex, mutex, signal};
// use embedded_graphics::{draw_target::DrawTarget, pixelcolor::BinaryColor};

// 1. 定义 LED 接口 (Trait)
// 任何能开关的东西都可以叫 Led，不管它是 GPIO 还是通过 CAN 扩展的


pub mod led;
pub mod colors;