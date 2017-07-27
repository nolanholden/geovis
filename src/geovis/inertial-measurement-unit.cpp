#include "inertial-measurement-unit.h"

#include "constants.h"
#include "printouts.h"

#include <functional>
#include <utility>
#include <vector>

namespace rcr {
namespace geovis {

namespace {
  constexpr const char* kImuDisplayName = "Inertial Measurement Unit";
  constexpr const char* kImuCsvHeader = "Heading/Euler-X (Magnetic),Pitch/Euler-Y,Roll/Euler-Z,Linear Accel (X),Linear Accel (Y),Linear Accel (Z),Gravity Accel (X),Gravity Accel (Y),Gravity Accel (Z),";
} // namespace

InertialMeasurementUnit::InertialMeasurementUnit()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR,
    kImuDisplayName, kImuCsvHeader) {
  quat_w_    = kalmanInit(0.);
  quat_x_    = kalmanInit(0.);
  quat_y_    = kalmanInit(0.);
  quat_z_    = kalmanInit(0.);
  linear_x_  = kalmanInit(0.);
  linear_y_  = kalmanInit(0.);
  linear_z_  = kalmanInit(0.);
  gravity_x_ = kalmanInit(0.);
  gravity_y_ = kalmanInit(0.);
  gravity_z_ = kalmanInit(0.);
}

bool InertialMeasurementUnit::Init() {
  return (init_result_ = bno_.begin());
}

imu::Vector<3> InertialMeasurementUnit::GetOrientation() {
  UpdateOrientation();
  auto euler = imu::Quaternion{
    quat_w_.value,
    quat_x_.value,
    quat_y_.value,
    quat_z_.value
  
  }.toEuler(); // radians

  euler.toDegrees(); // use degrees.
  return euler;
}

imu::Vector<3> InertialMeasurementUnit::GetLinearAccel() {
  UpdateLinearAccel();
  auto v = imu::Vector<3>(linear_x_.value, linear_y_.value, linear_z_.value);
  return v;
}

imu::Vector<3> InertialMeasurementUnit::GetGravitationalAccel() {
  UpdateGravitationalAccel();
  auto v = imu::Vector<3>(gravity_x_.value, gravity_y_.value, gravity_z_.value);
  return v;
}

void InertialMeasurementUnit::UpdateOrientation() {
  auto quaternion = bno_.getQuat(); // radians
  kalmanUpdate(&quat_w_, std::move(quaternion.w()));
  kalmanUpdate(&quat_x_, std::move(quaternion.x()));
  kalmanUpdate(&quat_y_, std::move(quaternion.y()));
  kalmanUpdate(&quat_z_, std::move(quaternion.z()));
}

void InertialMeasurementUnit::UpdateLinearAccel() {
  auto linear = bno_.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  kalmanUpdate(&linear_x_, std::move(linear.x()));
  kalmanUpdate(&linear_y_, std::move(linear.y()));
  kalmanUpdate(&linear_z_, std::move(linear.z()));
}

void InertialMeasurementUnit::UpdateGravitationalAccel() {
  auto gravity = bno_.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
  kalmanUpdate(&gravity_x_, std::move(gravity.x()));
  kalmanUpdate(&gravity_y_, std::move(gravity.y()));
  kalmanUpdate(&gravity_z_, std::move(gravity.z()));
}

void InertialMeasurementUnit::UpdateAll() {
  UpdateOrientation();
  UpdateLinearAccel();
  UpdateGravitationalAccel();
}

String InertialMeasurementUnit::GetCsvLine() {
  UpdateAll();

  String line = "";

  auto euler_degrees = GetOrientation();

  line += std::move(euler_degrees.x());
  line += ",";
  line += std::move(euler_degrees.y());
  line += ",";
  line += std::move(euler_degrees.z());
  line += ",";

  line += linear_x_.value;
  line += ",";
  line += linear_y_.value;
  line += ",";
  line += linear_z_.value;
  line += ",";

  line += gravity_x_.value;
  line += ",";
  line += gravity_y_.value;
  line += ",";
  line += gravity_z_.value;
  line += ",";

  return line;
}

// Calibrate the BNO055 IMU.
// Note: This function is provided within this project because calibration
// only persists for a single power-on (no onboard EEPROM).
void InertialMeasurementUnit::Calibrate() {
  Serial.println("Enter 'q' at any time to quit.");
  delay(1024u);

  uint8_t system, gyroscope, accelerometer, magnometer = 0;

  // Wait for 10 consecutive successes.
  auto numSuccesses = 0;
  while (numSuccesses < 10) {
    // Get the four calibration values (value = 0, 1, 2, 3, where 3 indicates
    // full calibration). Any sensor reporting 0 should be ignored.
    bno_.getCalibration(&system, &gyroscope, &accelerometer, &magnometer);

    // Print the values.
    Serial.print(" System: ");
    Serial.print(system);
    Serial.print(" Gyro: ");
    Serial.print(gyroscope);
    Serial.print(" Accelerometer: ");
    Serial.print(accelerometer);
    Serial.print(" Magnometer: ");
    Serial.println(magnometer);

    // Only when everybody is 3, increment our successes.
    if (gyroscope > 2 && accelerometer > 2 && magnometer > 2) {
      numSuccesses++;
    }
    else { // Only allow consective successes.
      numSuccesses = 0;
    }

    // Wait 100ms, while allowing user to quit at any time.
    auto start = millis();
    do {
      while (Serial.available() > 0) {
        if (Serial.read() == 'q') return;
      }
    } while (millis() - start < 100);
  }

  Serial.println();
  printouts::print_with_ellipses("IMU is now fully calibrated");
}

bool InertialMeasurementUnit::IsFullyCalibrated() {
  return bno_.isFullyCalibrated();
}

void InertialMeasurementUnit::Update() {
  UpdateAll();
}

} // namespace geovis
} // namespace rcr
