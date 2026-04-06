use bsw::bh1750;
use embedded_hal::i2c::I2c;

pub async fn init<E>(
    bh1750: &mut bh1750::Bh1750<impl I2c<Error = E>>
) -> Result<(), bh1750::Error<E>> {
    bh1750.init(bh1750::Mode::ContinuousHighRes).await
}

pub fn read<E>(
    bh1750: &mut bh1750::Bh1750<impl I2c<Error = E>>
) -> Result<f32, bh1750::Error<E>> {
    bh1750.read_lux()
}
