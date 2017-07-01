#include "thermometer.h"

namespace rcr {
namespace level1payload {

bool Thermometer::Init(std::shared_ptr<Adafruit_BME280> thermometer) {
  bme_ = thermometer;
  return bme_->begin();
}

float Thermometer::temperature() {
  return bme_->readTemperature();
}

} // namespace level1_payload
} // namespace rcr
