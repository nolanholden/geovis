#ifndef RCR_GEOVIS_IMU_H_
#define RCR_GEOVIS_IMU_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"

#include <Adafruit_BNO055.h>

namespace rcr {
namespace geovis {

class InertialMeasurementUnit : public virtual Sensor {
 public:
  InertialMeasurementUnit();

  bool Init();

  // Sample the given type of vector only once.
  imu::Vector<3> SampleVector(
    Adafruit_BNO055::adafruit_vector_type_t vector_type);

  // TODO: Is possible overflow an issue?
  // Sample the given type of vector, returning the mean values.
  template <int SampleSize>
  imu::Vector<3> SampleForMeanVector(
    Adafruit_BNO055::adafruit_vector_type_t vector_type);

  static constexpr const char* kCsvHeader = "x(heading),y(roll),z(pitch),Lx,Ly,Lz,Gx,Gy,Gz,";

  String GetCsvLine();

  ~InertialMeasurementUnit() {}

 private:
  static Adafruit_BNO055 bno_;
  
  // Orientation (Euler vector):
  kalman_t euler_x_; // heading
  kalman_t euler_y_; // roll
  kalman_t euler_z_; // pitch

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

#endif // RCR_GEOVIS_IMU_H_
