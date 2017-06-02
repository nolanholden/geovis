/* BMP180 setup for Arduino UNO:
 * ----------------------------------------
 * Connect BMP180 V-in   to 3.3V or 5.0V
 * Connect BMP180 GND    to Ground
 * Connect BMP180 SCL    to Analog 5
 * Connect BMP180 SDA    to Analog 4
 * ----------------------------------------
*/

#include <Wire.h>
#include <Adafruit_BMP085.h>

// begin SD card libraries
#include <BlockDriver.h>
#include <FreeStack.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SysCall.h>
// end SD card libraries

File file; // file object
SdFatSdio sd_card; // MicroSD card
Adafruit_BMP085 bmp;
  
void setup() {
  // Start serial communication.
  Serial.begin(9600); // in bits/second

  // Start the BMP-180. Trap the thread if no sensor is found.
  if (!bmp.begin()) {
  	Serial.println("No BMP sensor found. Program will not proceed.");
  	while (1) { /* Trap the thread. */ }
  }
}

void writeBmpData() {}

struct BmpData {
  
  // Temperature.
  // // UNIT: degrees Celcius
  float temperature;

  // Pressure at the sensor.
  // UNIT: pascal (N/m^2)
  int32_t ambient_pressure;
  
  // Pressure altitude: altitude with altimeter setting at 101325 Pascals == 1013.25 millibars
  // == 29.92 inches mercury (i.e., std. pressure) // For pressure conversions, visit NOAA 
  // at: https://www.weather.gov/media/epz/wxcalc/pressureConversion.pdf.
  // (Pressure altitude is NOT correct for non-standard pressure or temperature.)
  // UNIT: meters
  float pressure_altitude;

  // Only if a humidity sensor is viable, TODO (Nolan Holden):
  // Add density altitude:
  // pressure altitude corrected for nonstandard temperature.
  // Remember: higher density altitude (High, Hot, and Humid) means decreased performance.

  BmpData(float _temperature, int32_t _ambient_pressure, float _pressure_altitude) {
    temperature = _temperature;
    ambient_pressure = _ambient_pressure;
    pressure_altitude = _pressure_altitude;
  }
};

BmpData& getBmpData(void) {
  return BmpData(bmp.readTemperature(), bmp.readPressure(), bmp.readAltitude());
}

void printBmpData(void) {

  // Get temperature.
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" °C");

  // Get pressure at sensor.
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");


  Serial.print("Pressure altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");



  Serial.println();
}

void loop() {
  printBaroData();
  delay(1000);
}
