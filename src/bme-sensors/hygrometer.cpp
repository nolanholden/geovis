#include "hygrometer.h"

namespace rcr {
namespace level1payload {

bool Hygrometer::Init(std::shared_ptr<Adafruit_BME280> hygrometer) {
  bme_ = hygrometer;
  return bme_->begin();
}

float Hygrometer::humidity() {
  return bme_->readHumidity();
}

} // namespace level1_payload
} // namespace rcr
