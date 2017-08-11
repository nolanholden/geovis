#ifndef RCR_GEOVIS_ATMOSPHERICSENSOR_H_
#define RCR_GEOVIS_ATMOSPHERICSENSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "constants.h"
#include "kalman.h"
#include "sensor.h"
#include "updateable.h"

#include <Adafruit_BME280.h>

namespace rcr {
namespace geovis {

constexpr uint8_t BME_SCK = 13;
constexpr uint8_t BME_MISO = 1;
constexpr uint8_t BME_MOSI = 0;
constexpr uint8_t BME_CS = 31;

extern const float
  kAtmosphericKalmanProcessNoise,
  kAtmosphericKalmanMeasurementNoise,
  kAtmosphericKalmanError;

constexpr float kStdPressure = 1013.25f; // hecto-Pascals

class AtmosphericSensor : public Sensor, public Updateable {
 public:
  AtmosphericSensor();

  String GetCsvLine();

  void Update();

  // Pressure at the sensor.
  // [Pascal] (N/m^2)
  float ambient_pressure() { return bme_.readPressure(); }

  // Realtive Humidity
  // [%]
  float humidity() { return bme_.readHumidity(); }

  // Pressure altitude: altitude with altimeter setting at 101325
  // Pascals == 1013.25 millibars == 29.92 inches mercury (i.e., std. pressure)
  // For pressure conversions, visit NOAA at:
  // https://www.weather.gov/media/epz/wxcalc/pressureConversion.pdf.
  // (Pressure altitude is NOT correct for non-standard pressure or temperature.)
  // [meters]
  float pressure_altitude() { return bme_.readAltitude(kStdPressure); }

  // Temperature.
  // [degrees Celcius]
  float temperature() { return bme_.readTemperature(); }

  // TODO: Add density altitude:
  // (pressure altitude corrected for nonstandard temperature.)
  // Remember: higher density altitude (High, Hot, and Humid) means decreased performance.
  ~AtmosphericSensor() {}

 private:
   bool ProtectedInit() { return bme_.begin(); }

  Adafruit_BME280 bme_{ BME_CS, BME_MOSI, BME_MISO, BME_SCK }; // software SPI
  
  Kalman<float, kAtmosphericKalmanProcessNoise,
    kAtmosphericKalmanMeasurementNoise,
    kAtmosphericKalmanError>
    ambient_pressure_, humidity_, pressure_altitude_, temperature_;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_ATMOSPHERICSENSOR_H_
