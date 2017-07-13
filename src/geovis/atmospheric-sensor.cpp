#include "atmospheric-sensor.h"

#include "constants.h"

namespace rcr {
namespace geovis {

namespace {
  constexpr const char* const kAtmDisplayName = "Atmospheric Sensor";
  constexpr const char* const KAtmCsvHeader = "Ambient Temperature (*Celcius), Ambient Pressure (Pascal) [filtered], Ambient Pressure (Pascal) [raw], Relative Humidity (%), Altitude (meters) [filtered], Altitude (meters) [raw]";

  constexpr float std_pressure = 1013.25f; // hecto-Pascals
} // namespace

AtmosphericSensor::AtmosphericSensor()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR,
    kAtmDisplayName, KAtmCsvHeader) {
  altitude_ = kalmanInit(0.);
  pressure_ = kalmanInit(0.);
}

bool AtmosphericSensor::Init() {
  init_result_ = bme_.begin();
  return init_result_;
}

double AtmosphericSensor::ambient_pressure() {
  kalmanUpdate(&pressure_, ambient_pressure_raw());
  return pressure_.value;
}

float AtmosphericSensor::ambient_pressure_raw() {
  return bme_.readPressure();
}

float AtmosphericSensor::humidity() {
  return bme_.readHumidity();
}

double AtmosphericSensor::pressure_altitude() {
  // @ std. pressure (i.e., 101325 Pa)
  kalmanUpdate(&altitude_, pressure_altitude_raw());
  return altitude_.value; 
}

float AtmosphericSensor::pressure_altitude_raw() {
  return bme_.readAltitude(std_pressure); // std. pressure
}

float AtmosphericSensor::temperature() {
  return bme_.readTemperature();
}

String AtmosphericSensor::GetCsvLine() {
  String line = "";

  // Temperature (*C)
  line += temperature();
  line += ",";

  // Ambient pressure (Pascals)
  line += ambient_pressure();
  line += ",";
  line += ambient_pressure_raw();
  line += ",";

  // Relative humidity (%)
  line += humidity();
  line += ",";

  // Pressure altitude (meters)
  line += pressure_altitude();
  line += ",";
  line += pressure_altitude_raw();
  line += ",";

  return line;
}

} // namespace geovis
} // namespace rcr
