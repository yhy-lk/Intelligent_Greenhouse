use bsw::sht30;
use embedded_hal::i2c::I2c;

pub async fn init<E>(
    sht30: &mut sht30::Sht30<impl I2c<Error = E>>
) -> Result<(), sht30::Error<E>> {
    sht30.init().await
}

pub async fn read<E>(
    sht30: &mut sht30::Sht30<impl I2c<Error = E>>
) -> Result<sht30::Measurement, sht30::Error<E>> {
    sht30.measure().await
}


