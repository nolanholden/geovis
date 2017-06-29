#ifndef RCR_LEVEL1PAYLOAD_BMP180_H_
#define RCR_LEVEL1PAYLOAD_BMP180_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"

#include <Adafruit_BME280.h>

#include <memory>

namespace rcr {
namespace level1payload {

class Barometer : public virtual Sensor {
 public:
  Barometer(void) {}

  // Temperature.
  // // UNIT: degrees Celcius
  float temperature(void);

  // Pressure at the sensor.
  // UNIT: Pascal (N/m^2)
  int32_t ambient_pressure(void);
  
  // Pressure altitude: altitude with altimeter setting at 101325 Pascals == 1013.25 millibars
  // == 29.92 inches mercury (i.e., std. pressure) // For pressure conversions, visit NOAA 
  // at: https://www.weather.gov/media/epz/wxcalc/pressureConversion.pdf.
  // (Pressure altitude is NOT correct for non-standard pressure or temperature.)
  // UNIT: meters
  float pressure_altitude(void);

  // Only if a humidity sensor is viable, TODO (Nolan Holden):
  // Add density altitude:
  // pressure altitude corrected for nonstandard temperature.
  // Remember: higher density altitude (High, Hot, and Humid) means decreased performance.

 private:
   std::shared_ptr<Adafruit_BME280> bmp_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_BMP180_H_
