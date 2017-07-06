#include "atmospheric-sensor.h"

#include "constants.h"

namespace rcr {
namespace level1payload {

AtmosphericSensor::AtmosphericSensor()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR) {
  altitude_ = kalmanInit(0.);
  pressure_ = kalmanInit(0.);
}

bool AtmosphericSensor::Init() {
  return bme_.begin();
}

float AtmosphericSensor::ambient_pressure() {
  kalmanUpdate(&pressure_, bme_.readPressure());
  return pressure_.value;
}

float AtmosphericSensor::ambient_pressure_raw() {
  return bme_.readPressure();
}

float AtmosphericSensor::humidity() {
  return bme_.readHumidity();
}

float AtmosphericSensor::pressure_altitude() {
  // @ std. pressure (i.e., 101325 Pa)
  kalmanUpdate(&altitude_, bme_.readAltitude(1013.25f));
  return altitude_.value; 
}

float AtmosphericSensor::pressure_altitude_raw() {
  return bme_.readAltitude(1013.25f);
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

  // Relative humidity (%)
  line += humidity();
  line += ",";

  // Pressure altitude (meters)
  line += pressure_altitude(); // 101325 Pa (std pressure)
  line += ",";

  return line;
}

} // namespace level1_payload
} // namespace rcr
