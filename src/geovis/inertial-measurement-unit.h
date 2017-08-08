#ifndef RCR_GEOVIS_IMU_H_
#define RCR_GEOVIS_IMU_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"
#include "updateable.h"

#include <Adafruit_BNO055.h>

namespace rcr {
namespace geovis {

class InertialMeasurementUnit : public Sensor, public Updateable {
 public:
  InertialMeasurementUnit();

  bool Init();

  // Calibrate the BNO055 IMU.
  // Note: This function is provided within this project because calibration
  // only persists for a single power-on (no onboard EEPROM).
  void Calibrate();

  // Get Euler angles vector [in degrees] (using quaterion).
  imu::Vector<3> GetOrientation();

  // Get linear acceleration vector.
  imu::Vector<3> GetLinearAccel();

  // Get gravitational acceleration vector.
  imu::Vector<3> GetGravitationalAccel();

  String GetCsvLine();

  bool IsFullyCalibrated();

  void Update();

  ~InertialMeasurementUnit();

 private:
  // Update, return nothing.
  void UpdateOrientation();

  // Update, return nothing.
  void UpdateLinearAccel();
  
  // Update, return nothing.
  void UpdateGravitationalAccel();

  // Update everything.
  void UpdateAll();

  Adafruit_BNO055 bno_;

  // Orientation quaternion:
  kalman_t quat_w_, quat_x_, quat_y_, quat_z_;

  // Linear accelleration:
  kalman_t linear_x_, linear_y_, linear_z_;

  // Gravitational accelleration:
  kalman_t gravity_x_, gravity_y_, gravity_z_;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_IMU_H_
