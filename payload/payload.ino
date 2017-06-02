#include <Wire.h>
#include "Adafruit_BMP085.h"

/* 
 * BMP180 setup instructions:
 * ----------------------------------------
 * Connect BMP180 V-in   to 3.3V (NOT 5.0V)
 * Connect BMP180 GND    to Ground
 * Connect BMP180 SCL    to Analog 5
 * Connect BMP180 SDA    to Analog 4
 * ----------------------------------------
*/

Adafruit_BMP085 barometer;
  
void setup() {
  Serial.begin(9600); // 9600 bits/second
  if (!barometer.begin()) {
  	Serial.println("No BMP085 sensor found.");
  	while (1) { /* Trap the thread. */ }
  }
}
  
void printBaroData() {

    // Get temperature.
    Serial.print("Temperature = ");
    Serial.print(barometer.readTemperature());
    Serial.println(" *C");

    // Get pressure at sensor.
    Serial.print("Pressure = ");
    Serial.print(barometer.readPressure());
    Serial.println(" Pa");

    // Calculate pressure at 0 MSL. (0 meters mean sea-level)
    Serial.print("Calculated pressure at 0 MSL = ");
    Serial.print(barometer.readSealevelPressure());
    Serial.println(" Pa");

    // Get pressure altitude:
    // altitude with (default) altimeter setting at 101325 Pascals == 1013.25 millibars
    // == 29.92 inches mercury (i.e., std. pressure)
    Serial.print("Pressure altitude = ");
    Serial.print(barometer.readAltitude());
    Serial.println(" meters");

    // TODO:
    // Density altitude:
    // pressure altitude corrected for nonstandard temperature
    // High density altitude (High, Hot, and Humid) means decreased performance.

    // Get indicated altitude:
    // pressure altitude corrected for non-standard pressure, with altimeter 
    // setting 1015 millibars == 101500 Pascals
    // For pressure conversions, visit NOAA at: https://www.weather.gov/media/epz/wxcalc/pressureConversion.pdf.
    Serial.print("Indicated altitude = ");
    Serial.print(barometer.readAltitude(101500));
    Serial.println(" meters");

    Serial.println();
}
void loop() {
    printBaroData();
    delay(350);
}
