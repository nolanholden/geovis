#include "inertial-measurement-unit.h"

#include "constants.h"
#include "printouts.h"

#include <utility>
#include <cmath>

namespace rcr {
namespace geovis {

constexpr double kImuKalmanProcessNoise = 0.01;
constexpr double kImuKalmanMeasurementNoise = 0.25;
constexpr double kImuKalmanError = 1.;
static_assert(kImuKalmanProcessNoise
  + kImuKalmanMeasurementNoise
  + kImuKalmanError != double{ 0 },
  "The sum of 'process noise covariance', 'measurement noise covariance', and 'estimation error covariance' cannot be zero; this creates a divide-by-zero condition.");

namespace {
  constexpr const char* kImuDisplayName = "Inertial Measurement Unit";
  constexpr const char* kImuCsvHeader = "Heading (magnetic),Pitch/Euler-Y,Roll/Euler-Z,Linear Accel (X),Linear Accel (Y),Linear Accel (Z),Linear Accel Magnitude,Gravity Accel (X),Gravity Accel (Y),Gravity Accel (Z),";
} // namespace

InertialMeasurementUnit::InertialMeasurementUnit()
  : Sensor(kImuDisplayName, kImuCsvHeader) {}

void InertialMeasurementUnit::Update() {
  linear_accel_ = bno_.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  linear_x_.Update(linear_accel_.x());
  linear_y_.Update(linear_accel_.y());
  linear_z_.Update(linear_accel_.z());

  gravitational_accel_ = bno_.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
  gravity_x_.Update(gravitational_accel_.x());
  gravity_y_.Update(gravitational_accel_.y());
  gravity_z_.Update(gravitational_accel_.z());

  orientation_ = bno_.getQuat(); // radians
  quat_w_.Update(orientation_.w());
  quat_x_.Update(orientation_.x());
  quat_y_.Update(orientation_.y());
  quat_z_.Update(orientation_.z());
}

imu::Vector<3> InertialMeasurementUnit::GetOrientationEuler() const {
  auto euler = orientation().toEuler(); // in radians
  euler.toDegrees(); // use degrees.
  return euler;
}

inline double InertialMeasurementUnit::GetAccelerationMagnitude() const {
  auto mag = std::sqrt(
      linear_accel().x()
    + linear_accel().y()
    + linear_accel().z()
  );
  return mag;
}

String InertialMeasurementUnit::GetCsvLine() {
  String line = "";

  auto euler_degrees = GetOrientationEuler();

  line += std::move(euler_degrees.x());
  line += ",";
  line += std::move(euler_degrees.y());
  line += ",";
  line += std::move(euler_degrees.z());
  line += ",";

  line += linear_accel().x();
  line += ",";
  line += linear_accel().y();
  line += ",";
  line += linear_accel().z();
  line += ",";
  line += GetAccelerationMagnitude();
  line += ",";

  line += gravitational_accel().x();
  line += ",";
  line += gravitational_accel().y();
  line += ",";
  line += gravitational_accel().z();
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

} // namespace geovis
} // namespace rcr
