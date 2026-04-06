#![no_std]

use core::fmt::Write;
// use defmt::info;

use display_interface::DisplayError;
use embedded_graphics::{
    mono_font::{MonoTextStyleBuilder, iso_8859_10::FONT_6X10},
    pixelcolor::BinaryColor,
    prelude::*,
    text::{Baseline, Text},
};
use heapless::String;
use service::led::BoardLed;

pub mod led;
pub mod ui;
pub mod ws2812;
pub mod sht30;
pub mod bh1750;