#include "Bmp180.h"

namespace rcr {
namespace level1payload {

void Bmp180::Setup(void) {
  Serial.println("Setting up BMP180...");
  
  // Start the BMP-180. Trap the thread if no sensor is found.
  if (!bmp_.begin()) {
    Serial.println("No BMP sensor found. Program will not proceed.");
    while (1) { /* Trap the thread. */ }
  }
  
  Serial.println("BMP180 successfully setup.");
  Serial.println();
}

float Bmp180::temperature(void) {
  return bmp_.readTemperature();
}

int32_t Bmp180::ambient_pressure(void) {
  return bmp_.readPressure();
}

float Bmp180::pressure_altitude(void) {
  // Use default altimeter setting, 
  // sealevelPressure = 101325 Pascals (std. atmosphere)
  return bmp_.readAltitude();
}

} // namespace level1_payload
} // namespace rcr
