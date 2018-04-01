#include "atmospheric-sensor.h"

#include "constants.h"
#include "sensor.h"

namespace rcr {
namespace geovis {

namespace {
  constexpr const char* const kAtmDisplayName = "Atmospheric Sensor";
  constexpr const char* const KAtmCsvHeader = "Ambient Temperature (*Celcius),Ambient Pressure (Pascal),Relative Humidity (%),Pressure Altitude (meters),";
} // namespace

AtmosphericSensor::AtmosphericSensor()
  : Sensor(kAtmDisplayName, KAtmCsvHeader) {}

void AtmosphericSensor::Update() {

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
