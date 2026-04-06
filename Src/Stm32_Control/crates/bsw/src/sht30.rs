// src/sht30.rs

// 引入 embedded-hal 1.0 标准 I2c Trait
use embedded_hal::i2c::I2c;
use embassy_time::Timer;

/// SHT30 I2C 地址枚举
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Sht30Address {
    Addr44 = 0x44, // ADDR -> GND
    Addr45 = 0x45, // ADDR -> VDD
}

impl From<Sht30Address> for u8 {
    fn from(addr: Sht30Address) -> Self {
        addr as u8
    }
}

/// 测量结果结构体
#[derive(Debug, Clone, Copy)]
pub struct Measurement {
    pub temp_c: f32, // 温度 (°C)
    pub hum_rh: f32, // 湿度 (%RH)
}

/// SHT30 驱动错误枚举
#[derive(Debug, defmt::Format)]
pub enum Error<E> {
    I2c(E),         // 底层 I2C 错误 (NACK, Timeout 等)
    Crc,            // CRC 校验失败
    InvalidData,    // 数据逻辑错误 (全0或全FF)
}

/// SHT30 驱动结构体
pub struct Sht30<I> {
    i2c: I,
    addr: u8,
}

impl<I, E> Sht30<I>
where
    // embedded-hal 1.0 中，I2c Trait 包含了 read/write/write_read
    I: I2c<Error = E>, 
{
    /// 创建新实例
    pub fn new(i2c: I, addr: Sht30Address) -> Self {
        Self {
            i2c,
            addr: addr.into(),
        }
    }

    /// 初始化 (软复位)
    pub async fn init(&mut self) -> Result<(), Error<E>> {
        // 发送软复位命令 0x30A2
        self.i2c
            .write(self.addr, &[0x30, 0xA2])
            .map_err(Error::I2c)?;
        
        // 软复位需要时间，10ms 足够安全
        Timer::after_millis(10).await;
        Ok(())
    }

    /// 执行单次测量
    pub async fn measure(&mut self) -> Result<Measurement, Error<E>> {
        // 1. 发送测量命令 (阻塞写)
        // 0x2400: High repeatability, no clock stretching
        self.i2c
            .write(self.addr, &[0x24, 0x00])
            .map_err(Error::I2c)?;

        // 2. 异步等待转换
        // SHT30 High 模式最大需 15ms，给 20ms 余量
        Timer::after_millis(20).await;

        // 3. 读取数据 (阻塞读)
        // 数据结构: [TempMsb, TempLsb, TempCrc, HumMsb, HumLsb, HumCrc]
        let mut buf = [0u8; 6];
        self.i2c
            .read(self.addr, &mut buf)
            .map_err(Error::I2c)?;

        // 4. 数据完整性快速校验 (优化后：一次比较完成)
        // 这种写法会被编译器优化为 memcmp，效率最高
        if buf == [0x00; 6] || buf == [0xFF; 6] {
            return Err(Error::InvalidData);
        }

        // 5. CRC 校验 (核心逻辑)
        // 校验温度: buf[0..2] 是数据, buf[2] 是校验值
        Self::check_crc(&buf[0..2], buf[2])
            .map_err(|_| Error::Crc)?;

        // 校验湿度: buf[3..5] 是数据, buf[5] 是校验值
        Self::check_crc(&buf[3..5], buf[5])
            .map_err(|_| Error::Crc)?;


        // 6. 解析数据
        let temp_raw = u16::from_be_bytes([buf[0], buf[1]]);
        let hum_raw = u16::from_be_bytes([buf[3], buf[4]]);
  
        // 转换公式
        let temp_c = -45.0 + 175.0 * (temp_raw as f32 / 65535.0);
        let hum_rh = 100.0 * (hum_raw as f32 / 65535.0);

        Ok(Measurement { temp_c, hum_rh })
    }

    /// 内部使用的 CRC8 校验函数
    /// 算法: CRC-8/SENSIRION
    /// 多项式: 0x31 (x^8 + x^5 + x^4 + 1)
    /// 初始值: 0xFF
    fn check_crc(data: &[u8], expected_checksum: u8) -> Result<(), ()> {
        let mut crc: u8 = 0xFF;

        for &byte in data {
            crc ^= byte; // 把数据 XOR 到 CRC 低位

            for _ in 0..8 {
                if (crc & 0x80) != 0 {
                    // 如果最高位是 1，左移后 XOR 多项式
                    crc = (crc << 1) ^ 0x31;
                } else {
                    // 否则直接左移
                    crc <<= 1;
                }
            }
        }

        if crc == expected_checksum {
            Ok(())
        } else {
            Err(())
        }
    }
    
    /// 释放 I2C 总线所有权
    pub fn release(self) -> I {
        self.i2c
    }
}