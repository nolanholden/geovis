# payload-level1-rocket
Data acquisition payload for a Level 1 High-Power Rocket certification launch

## Formulas:

![](https://raw.githubusercontent.com/nolanholden/payload-level1-rocket/8447a1cbf741a6a57f07a59d258492eb7169c5b9/misc/pressure-altitude.png)

## Setup for components:

#### BME280 to Teensy 3.6:
- Connect BME280 Vin   to 3.3V
- Connect BME280 GND    to Ground
- Connect BME280 SCK    to SCL0
- Connect BME280 SDI    to SDA0

#### BMP180 to Arduino UNO:
- Connect BMP180 V-in   to 3.3V or 5.0V
- Connect BMP180 GND    to Ground
- Connect BMP180 SCL    to Analog 5
- Connect BMP180 SDA    to Analog 4
