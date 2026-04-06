
use embedded_hal::i2c::I2c;
use embassy_time::{Duration, Timer};

/// BH1750 I2C 地址枚举
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Bh1750Address {
    Addr23 = 0x23, // ADDR 引脚接地或悬空
    Addr5C = 0x5C, // ADDR 引脚接 VCC
}

impl From<Bh1750Address> for u8 {
    fn from(addr: Bh1750Address) -> Self {
        addr as u8
    }
}

/// BH1750 测量模式枚举
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Mode {
    ContinuousHighRes = 0x10, // 连续高分辨率模式 (1 Lux，转换时间最多 180ms)
    ContinuousLowRes = 0x13,  // 连续低分辨率模式 (4 Lux，转换时间最多 24ms)
}

/// BH1750 驱动错误枚举
#[derive(Debug, defmt::Format)]
pub enum Error<E> {
    I2c(E),         // 底层 I2C 错误
    InvalidData,    // 异常数据
}

/// BH1750 驱动结构体
pub struct Bh1750<I> {
    i2c: I,
    addr: u8,
}

impl<I, E> Bh1750<I>
where
    I: I2c<Error = E>,
{
    /// 创建新实例
    pub fn new(i2c: I, addr: Bh1750Address) -> Self {
        Self {
            i2c,
            addr: addr.into(),
        }
    }

    /// 初始化设备：发送通电指令并设置连续测量模式
    pub async fn init(&mut self, mode: Mode) -> Result<(), Error<E>> {
        // 1. 发送通电指令 0x01
        self.i2c.write(self.addr, &[0x01]).map_err(Error::I2c)?;
        
        // 2. 写入测量模式
        self.i2c.write(self.addr, &[mode as u8]).map_err(Error::I2c)?;

        // 3. 等待首次转换完成
        let wait_time = match mode {
            Mode::ContinuousHighRes => 180,
            Mode::ContinuousLowRes => 24,
        };
        Timer::after(Duration::from_millis(wait_time)).await;

        Ok(())
    }

    /// 读取光照强度 (Lux)
    /// 需在调用 init 后，按照指定的频率或周期进行调用
    pub fn read_lux(&mut self) -> Result<f32, Error<E>> {
        let mut buf = [0u8; 2];
        
        self.i2c.read(self.addr, &mut buf).map_err(Error::I2c)?;

        // 基础数据完整性校验
        if buf == [0x00, 0x00] || buf == [0xFF, 0xFF] {
            // 注意: 全0可能是极暗环境，全FF可能是传感器异常，视具体业务逻辑可在此调整
            // return Err(Error::InvalidData); 
        }

        // 合并高低字节
        let raw_level = ((buf[0] as u16) << 8) | (buf[1] as u16);

        // 转换为 Lux
        let lux = (raw_level as f32) / 1.2;

        Ok(lux)
    }

    /// 释放 I2C 总线所有权
    pub fn release(self) -> I {
        self.i2c
    }
}