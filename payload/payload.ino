#include <Wire.h>
#include <Adafruit_BMP085.h>

/* 
 * BMP180 setup instructions:
 * ----------------------------------------
 * Connect BMP180 V-in   to 3.3V (NOT 5.0V)
 * Connect BMP180 GND    to Ground
 * Connect BMP180 SCL    to Analog 5
 * Connect BMP180 SDA    to Analog 4
 * ----------------------------------------
*/

Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(2000000); // bits/second
  if (!bmp.begin()) {
  	Serial.println("No BMP085 sensor found. Program will not proceed.");
  	while (1) { /* Trap the thread. */ }
  }
}
  
void printBaroData() {

  // Get temperature.
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  // Get pressure at sensor.
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  // Calculate pressure at 0 MSL. (0 meters mean sea-level)
  Serial.print("Calculated pressure at 0 MSL = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");

  // Get pressure altitude:
  // altitude with (default) altimeter setting at 101325 Pascals == 1013.25 millibars
  // == 29.92 inches mercury (i.e., std. pressure)
  Serial.print("Pressure altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");

  // TODO:
  // Density altitude:
  // pressure altitude corrected for nonstandard temperature
  // High density altitude (High, Hot, and Humid) means decreased performance.

  // Get indicated altitude:
  // pressure altitude corrected for non-standard pressure, with 
  // altimeter setting (in Pascals) as argument 
  // (29.92 inches mercury == 101325 Pascals == 1013.25 millibars)
  // For pressure conversions, visit NOAA at: https://www.weather.gov/media/epz/wxcalc/pressureConversion.pdf.
  Serial.print("Indicated altitude =                                  ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");
  Serial.print("Indicated altitude with sensor pressure adjustment =  ");
  Serial.print(bmp.readAltitude((float)bmp.readPressure()));
  Serial.println(" meters");

  Serial.println();
}

void loop() {
  printBaroData();
  delay(200);
}
