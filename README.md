# Level-1 High-Power Rocket Payload
Data acquisition payload for a Level 1 High-Power Rocket certification launch

*This project draws much inspiration and several resources from River City Rocketry's [Variable Drag System](https://github.com/jtcass01/vds2_0) and Shane Tully's [Osprey](https://github.com/shanet/osprey/) project.*

## Notable formulas:

![](https://raw.githubusercontent.com/nolanholden/payload-level1-rocket/8447a1cbf741a6a57f07a59d258492eb7169c5b9/misc/pressure-altitude.png)

## Setup for components:

#### BNO055 to Teensy 3.6:
- Connect BNO055 Vin    to 3-5V
- Connect BNO055 GND    to Ground
- Connect BNO055 SCK    to SCL0
- Connect BNO055 SDI    to SDA0

#### BME280 to Teensy 3.6:
- Connect BME280 Vin    to 3.3V
- Connect BME280 GND    to Ground
- Connect BME280 SCK    to SCL1
- Connect BME280 SDI    to SDA1

#### BMP180 to Arduino UNO:
- Connect BMP180 Vin    to 3.3V or 5.0V
- Connect BMP180 GND    to Ground
- Connect BMP180 SCL    to Analog 5
- Connect BMP180 SDA    to Analog 4
