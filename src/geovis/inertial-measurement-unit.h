#ifndef RCR_GEOVIS_IMU_H_
#define RCR_GEOVIS_IMU_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "kalman.h"
#include "sensor.h"
#include "updateable.h"

#include <Adafruit_BNO055.h>

namespace rcr {
namespace geovis {

extern const double
  kImuKalmanProcessNoise,
  kImuKalmanMeasurementNoise,
  kImuKalmanError;

class InertialMeasurementUnit : public Sensor, public Updateable {
 public:
  InertialMeasurementUnit();
  
  // Get unit quaternion for vehicle orientation.
  const imu::Quaternion& orientation() const        { return orientation_; }

  const imu::Vector<3>& linear_accel() const        { return linear_accel_; }

  const imu::Vector<3>& gravitational_accel() const { return gravitational_accel_; }

  // Get Euler-angles vector in degrees (using quaterion mathematics).
  // x - yaw, y - pitch, z - roll
  imu::Vector<3> GetOrientationEuler() const;

  String GetCsvLine();

  void Update();

  // Calibrate the BNO055 IMU.
  // Note: This function is provided within this project because calibration
  // only persists for a single power-on (no onboard EEPROM).
  void Calibrate();

  bool IsFullyCalibrated() { return bno_.isFullyCalibrated(); }

  ~InertialMeasurementUnit() {}

 private:
  bool ProtectedInit() { return bno_.begin(); }

  Adafruit_BNO055 bno_;

  imu::Quaternion orientation_;
  imu::Vector<3> linear_accel_;
  imu::Vector<3> gravitational_accel_;

  // Orientation quaternion:
  Kalman<double, kImuKalmanProcessNoise, kImuKalmanMeasurementNoise, kImuKalmanError>
    quat_w_, quat_x_, quat_y_, quat_z_;

  // Linear accelleration:
  Kalman<double, kImuKalmanProcessNoise, kImuKalmanMeasurementNoise, kImuKalmanError>
    linear_x_, linear_y_, linear_z_;

  // Gravitational accelleration:
  Kalman<double, kImuKalmanProcessNoise, kImuKalmanMeasurementNoise, kImuKalmanError>
    gravity_x_, gravity_y_, gravity_z_;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_IMU_H_
