#include "atmospheric-sensor.h"

#include "constants.h"

namespace rcr {
namespace geovis {

Adafruit_BME280 AtmosphericSensor::bme_(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

AtmosphericSensor::AtmosphericSensor()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR,
    "Atmospheric Sensor") {
  altitude_ = kalmanInit(0.);
  pressure_ = kalmanInit(0.);
}

bool AtmosphericSensor::Init() {
  init_result_ = bme_.begin();
  return init_result_;
}

double AtmosphericSensor::ambient_pressure() {
  kalmanUpdate(&pressure_, bme_.readPressure());
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
  line += ambient_pressure_raw();
  line += ",";

  // Relative humidity (%)
  line += humidity();
  line += ",";

  // Pressure altitude (meters)
  line += pressure_altitude_raw(); // 101325 Pa (std pressure)
  line += ",";

  return line;
}

} // namespace geovis
} // namespace rcr
