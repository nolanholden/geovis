#include "inertial-measurement-unit.h"

#include "constants.h"
#include "novelty-printouts.h"

#include <vector>
#include <utility>

namespace rcr {
namespace geovis {

namespace {
  constexpr const char* kImuDisplayName = "Inertial Measurement Unit";
  constexpr const char* kImuCsvHeader = "x(heading),y(roll),z(pitch),Lx,Ly,Lz,Gx,Gy,Gz,";
} // namespace

InertialMeasurementUnit::InertialMeasurementUnit()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR,
    kImuDisplayName, kImuCsvHeader) {
  euler_x_   = kalmanInit(0.);
  euler_y_   = kalmanInit(0.);
  euler_z_   = kalmanInit(0.);

  // Linear accelleration:
  linear_x_  = kalmanInit(0.);;
  linear_y_  = kalmanInit(0.);;
  linear_z_  = kalmanInit(0.);;

  // Gravitational accelleration:
  gravity_x_ = kalmanInit(0.);;
  gravity_y_ = kalmanInit(0.);;
  gravity_z_ = kalmanInit(0.);;
}

bool InertialMeasurementUnit::Init() {
  init_result_ = bno_.begin();
  return init_result_;
}

// TODO: Is possible overflow an issue?
template <int SampleSize>
imu::Vector<3> InertialMeasurementUnit::SampleForMeanVector (
    Adafruit_BNO055::adafruit_vector_type_t vector_type) {
  // Resultant vector of arithmetic-mean x,y,z values.
  auto mean_vector = imu::Vector<3>();

  // Ensure x,y,z == 0.0
  mean_vector.x() = 0.;
  mean_vector.y() = 0.;
  mean_vector.z() = 0.;

  // Sample the IMU sensor.
  auto imu_vectors = std::vector<imu::Vector<3>>(static_cast<size_t>(SampleSize));
  for (auto i = 0; i < SampleSize; ++i) {
    imu_vectors.push_back(std::move(bno_.getVector(vector_type)));
  }

  // Compute the mean.
  auto sample_size_double = static_cast<double>(SampleSize);
  for (auto& v : imu_vectors) {
    mean_vector.x() += v.x();
    mean_vector.y() += v.y();
    mean_vector.z() += v.z();
  }
  mean_vector.x() /= sample_size_double;
  mean_vector.y() /= sample_size_double;
  mean_vector.z() /= sample_size_double;

  return mean_vector;
}

imu::Vector<3> InertialMeasurementUnit::SampleVector(
    Adafruit_BNO055::adafruit_vector_type_t vector_type) {
  return bno_.getVector(vector_type);
}

String InertialMeasurementUnit::GetCsvLine() {
  String line = "";

  //  Sample the IMU.
  auto orientation = bno_.getVector(Adafruit_BNO055::VECTOR_EULER);
  auto linear = bno_.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  auto gravity = bno_.getVector(Adafruit_BNO055::VECTOR_GRAVITY);

  // Update respective Kalman filters.
    // (1) Euler vector:
  kalmanUpdate(&euler_x_, orientation.x());
  kalmanUpdate(&euler_y_, orientation.y());
  kalmanUpdate(&euler_z_, orientation.z());

    // (2) Linear accelleration vector:
  kalmanUpdate(&linear_x_, linear.x());
  kalmanUpdate(&linear_y_, linear.y());
  kalmanUpdate(&linear_z_, linear.z());

    // (3) Gravitational accelleration vector:
  kalmanUpdate(&gravity_x_, gravity.x());
  kalmanUpdate(&gravity_y_, gravity.y());
  kalmanUpdate(&gravity_z_, gravity.z());

  // Return *filtered* results.
  line += euler_x_.value;
  line += ",";
  line += euler_y_.value;
  line += ",";
  line += euler_z_.value;
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
  print_with_ellipses("IMU is now fully calibrated");
}

bool InertialMeasurementUnit::IsFullyCalibrated() {
  return bno_.isFullyCalibrated();
}

} // namespace geovis
} // namespace rcr
