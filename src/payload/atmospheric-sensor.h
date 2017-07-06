#ifndef RCR_LEVEL1PAYLOAD_ATMOSPHERICSENSOR_H_
#define RCR_LEVEL1PAYLOAD_ATMOSPHERICSENSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"

#include <Adafruit_BME280.h>

namespace rcr {
namespace level1payload {

class AtmosphericSensor : public virtual Sensor {
 public:
  AtmosphericSensor();

  bool Init();

  // Pressure at the sensor.
  // [Pascal] (N/m^2)
  float ambient_pressure();
  float ambient_pressure_raw(); // unfiltered

  // Realtive Humidity
  // [%]
  float humidity();

  // Pressure altitude: altitude with altimeter setting at 101325 Pascals == 1013.25 millibars
  // == 29.92 inches mercury (i.e., std. pressure) // For pressure conversions, visit NOAA 
  // at: https://www.weather.gov/media/epz/wxcalc/pressureConversion.pdf.
  // (Pressure altitude is NOT correct for non-standard pressure or temperature.)
  // [meters]
  float pressure_altitude();
  float pressure_altitude_raw(); // unfiltered

  // Temperature.
  // [degrees Celcius]
  float temperature();

  // TODO: Add density altitude:
  // (pressure altitude corrected for nonstandard temperature.)
  // Remember: higher density altitude (High, Hot, and Humid) means decreased performance.

  static constexpr const char* kCsvHeader = "*C,Pa,%,m,";

  String GetCsvLine();

  ~AtmosphericSensor() {}

 private:
  // TODO: We want static, but is creating unknown 
  // ref (compilation) error in AtmosphericSensor::Init() { ... } .
  Adafruit_BME280 bme_;

  kalman_t altitude_;
  kalman_t pressure_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_ATMOSPHERICSENSOR_H_
