#ifndef RCR_LEVEL1PAYLOAD_SENSOR_H_
#define RCR_LEVEL1PAYLOAD_SENSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "kalman.h"

namespace rcr {
namespace level1payload {

class Sensor {
 public:
  Sensor() {}
  Sensor(double process_noise, double measurement_noise, double error);
  virtual bool Init() = 0;

  // Prevent base-call d'tor memory leak.
  virtual ~Sensor() noexcept {}
  
  // Disallow copying and moving.
  Sensor(const Sensor&) = delete;
  Sensor& operator=(const Sensor&) = delete;

 protected:
  kalman_t kalmanInit(double intial_value);
  void kalmanUpdate(kalman_t* state, double measurement);

 private:
  double processNoise_;
  double measurementNoise_;
  double error_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_SENSOR_H_
