#ifndef RCR_LEVEL1PAYLOAD_BAROMETER_H_
#define RCR_LEVEL1PAYLOAD_BAROMETER_H_

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

class Barometer : public virtual Sensor<std::shared_ptr<Adafruit_BME280>> {
 public:
  Barometer() {}

  bool Init(std::shared_ptr<Adafruit_BME280> barometer);

  // Pressure at the sensor.
  // UNIT: Pascal (N/m^2)
  float ambient_pressure();
  
  // Pressure altitude: altitude with altimeter setting at 101325 Pascals == 1013.25 millibars
  // == 29.92 inches mercury (i.e., std. pressure) // For pressure conversions, visit NOAA 
  // at: https://www.weather.gov/media/epz/wxcalc/pressureConversion.pdf.
  // (Pressure altitude is NOT correct for non-standard pressure or temperature.)
  // UNIT: meters
  float pressure_altitude();

  // Only if a humidity sensor is viable, TODO (Nolan Holden):
  // Add density altitude:
  // pressure altitude corrected for nonstandard temperature.
  // Remember: higher density altitude (High, Hot, and Humid) means decreased performance.

  ~Barometer() {}

 private:
  static std::shared_ptr<Adafruit_BME280> bme_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_BAROMETER_H_
