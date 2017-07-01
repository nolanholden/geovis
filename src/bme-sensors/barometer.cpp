#include "barometer.h"

#include <memory>

namespace rcr {
namespace level1payload {

bool Barometer::Init(std::shared_ptr<Adafruit_BME280> barometer) {
  bme_ = barometer;
  return bme_->begin();
}

float Barometer::ambient_pressure() {
  return bme_->readPressure();
}

float Barometer::pressure_altitude() {
  return bme_->readAltitude(1013.25f); // @ std. pressure (i.e., 101325 Pa)
}

} // namespace level1_payload
} // namespace rcr
