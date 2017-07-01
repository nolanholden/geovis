#include "bme.h"

namespace rcr {
namespace level1payload {

bool Bme::Init() {
  return bme_.begin();
}

float Bme::ambient_pressure() {
  return bme_.readPressure();
}

float Bme::humidity() {
  return bme_.readHumidity();
}

float Bme::pressure_altitude() {
  return bme_.readAltitude(1013.25f); // @ std. pressure (i.e., 101325 Pa)
}

float Bme::temperature() {
  return bme_.readTemperature();
}

} // namespace level1_payload
} // namespace rcr
