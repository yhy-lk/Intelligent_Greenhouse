

pub trait BoardLed {
    fn on(&mut self);
    fn off(&mut self);
    fn toggle(&mut self);
    fn toggle_count(&mut self) -> u32;
}