#ifndef RCR_LEVEL1PAYLOAD_THERMOMETER_H_
#define RCR_LEVEL1PAYLOAD_THERMOMETER_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"

#include <Adafruit_BME280.h>

#include <memory>

namespace rcr {
namespace level1payload {

class Thermometer : public virtual Sensor<std::shared_ptr<Adafruit_BME280>> {
 public:
  Thermometer() {}

  bool Init(std::shared_ptr<Adafruit_BME280> thermometer);

  // Temperature. (degrees Celcius)
  float temperature(void);

  ~Thermometer() {}

 private:
  static std::shared_ptr<Adafruit_BME280> bme_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_THERMOMETER_H_
