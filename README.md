# Level-1 High-Power Rocket Payload
Data acquisition payload for a Level 1 High-Power Rocket certification launch

*This project draws much inspiration and several resources from River City Rocketry's [Variable Drag System](https://github.com/jtcass01/vds2_0) and Shane Tully's [Osprey](https://github.com/shanet/osprey/) project.*

## Notable formulas:

![](https://raw.githubusercontent.com/nolanholden/payload-level1-rocket/8447a1cbf741a6a57f07a59d258492eb7169c5b9/misc/pressure-altitude.png)

## Components Setup:

#### Adafruit's "Ultimate GPS" module:
- Example wiring between module and Teensy 3.6:
  - GPS module RX -> Teensy 3.6 TX1 (pin 1)
  - GPS module TX -> Teensy 3.6 RX1 (pin 0)
  - Corresponding code:
    ```
    Serial1.setRX(0); // default for Serial1; unnecessary, but explicit
    Serial1.setTX(1); // default for Serial1; unnecessary, but explicit
    Serial1.begin(9600);
    // Note: All testing with Teensy 3.6 suggests that baud rates other
    // than 9600 are incompatible. However, 9600 is ok for 5 Hz GPS
    // refresh (which itself is very sufficient.)
    ```
- Default baud rate: 9600 Bd
- Default GPS sampling rate: 1 Hz

#### BNO055 Inertial Measurement Unit to Teensy 3.6:
- Example wiring between this IMU and Teensy 3.6:
  - BNO055 SDA -> Teensy 3.6 SDA0 (pin 18 / Analog 4)
  - BNO055 SCL -> Teensy 3.6 SCL0 (pin 19 / Analog 5)

#### BME280 Barometer/Thermometer/Hygrometer to Teensy 3.6:
- Example wiring between BME280 and Teensy 3.6:
  - BME280 SDI -> Teensy 3.6 SDA0 (pin 18 / Analog 4)
  - BME280 SCK -> Teensy 3.6 SCL0 (pin 19 / Analog 5)

#### BMP180 Barometer/Thermometer to Arduino UNO:
- Connect BMP180 SCL    to Analog 5
- Connect BMP180 SDA    to Analog 4
