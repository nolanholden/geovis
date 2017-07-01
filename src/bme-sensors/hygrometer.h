#ifndef RCR_LEVEL1PAYLOAD_HYGROMETER_H_
#define RCR_LEVEL1PAYLOAD_HYGROMETER_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"
#include "thermometer.h"

#include <Adafruit_BME280.h>

namespace rcr {
namespace level1payload {

class Hygrometer : public virtual Sensor<std::shared_ptr<Adafruit_BME280>> {
 public:
  Hygrometer() {}

  bool Init(std::shared_ptr<Adafruit_BME280> hygrometer);

  float humidity();

  ~Hygrometer() {}

 private:
  static std::shared_ptr<Adafruit_BME280> bme_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_HYGROMETER_H_
