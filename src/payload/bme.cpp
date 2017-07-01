#include "bme.h"

namespace rcr {
namespace level1payload {

Bme::Bme()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR) {
  altitude_ = kalmanInit(0);
  pressure_ = kalmanInit(0);
}

bool Bme::Init() {
  return bme_.begin();
}

float Bme::ambient_pressure() {
  kalmanUpdate(&pressure_, bme_.readPressure());
  return pressure_.value;
}

float Bme::ambient_pressure_raw() {
  return bme_.readPressure();
}

float Bme::humidity() {
  return bme_.readHumidity();
}

float Bme::pressure_altitude() {
  // @ std. pressure (i.e., 101325 Pa)
  kalmanUpdate(&altitude_, bme_.readAltitude(1013.25f));
  return altitude_.value; 
}

float Bme::pressure_altitude_raw() {
  return bme_.readAltitude(1013.25f);
}

float Bme::temperature() {
  return bme_.readTemperature();
}

} // namespace level1_payload
} // namespace rcr
