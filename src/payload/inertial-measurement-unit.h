#ifndef RCR_LEVEL1PAYLOAD_IMU_H_
#define RCR_LEVEL1PAYLOAD_IMU_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"

#include <Adafruit_BNO055.h>

namespace rcr {
namespace level1payload {

class InertialMeasurementUnit : public virtual Sensor {
 public:
  InertialMeasurementUnit();

  bool Init();

  // TODO: Is possible overflow an issue?
  // Gets the mean x,y,z values of the given IMU vector for a given number of samples.
  template <int SampleSize>
  imu::Vector<3> SampleForMeanVector (
    Adafruit_BNO055::adafruit_vector_type_t vector_type);

  void GetCsvLine(String* string_to_append);

  ~InertialMeasurementUnit() {}

 private:
  // TODO: We want static, but is creating unknown 
  // ref (compilation) error in AtmosphericSensor::Init() { ... } .
  static Adafruit_BNO055 bno_;
  
  // Orientation (Euler vector):
  kalman_t euler_x_; // heading
  kalman_t euler_y_;    // roll
  kalman_t euler_z_;   // pitch

  // Linear accelleration:
  kalman_t linear_x_;
  kalman_t linear_y_;
  kalman_t linear_z_;

  // Gravitational accelleration:
  kalman_t gravity_x_;
  kalman_t gravity_y_;
  kalman_t gravity_z_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_IMU_H_
