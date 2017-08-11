#include "atmospheric-sensor.h"

#include "constants.h"
#include "sensor.h"

namespace rcr {
namespace geovis {

constexpr float kAtmosphericKalmanProcessNoise = 0.01f;
constexpr float kAtmosphericKalmanMeasurementNoise = 0.25f;
constexpr float kAtmosphericKalmanError = 1.f;
static_assert(kAtmosphericKalmanProcessNoise
  + kAtmosphericKalmanMeasurementNoise
  + kAtmosphericKalmanError != float{ 0 },
  "The sum of 'process noise covariance', 'measurement noise covariance', and 'estimation error covariance' cannot be zero; this creates a divide-by-zero condition.");

namespace {
  constexpr const char* const kAtmDisplayName = "Atmospheric Sensor";
  constexpr const char* const KAtmCsvHeader = "Ambient Temperature (*Celcius),Ambient Pressure (Pascal),Relative Humidity (%),Pressure Altitude (meters),";
} // namespace

AtmosphericSensor::AtmosphericSensor()
  : Sensor(kAtmDisplayName, KAtmCsvHeader) {}

void AtmosphericSensor::Update() {
  ambient_pressure_.Update(ambient_pressure());
  humidity_.Update(humidity());
  pressure_altitude_.Update(pressure_altitude());
  temperature_.Update(temperature());
}

String AtmosphericSensor::GetCsvLine() {
  String line = "";

  line += temperature();
  line += ",";

  line += ambient_pressure();
  line += ",";

  line += humidity();
  line += ",";

  line += pressure_altitude();
  line += ",";

  return line;
}

} // namespace geovis
} // namespace rcr
