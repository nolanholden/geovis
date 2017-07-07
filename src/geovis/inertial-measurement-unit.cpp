#include "inertial-measurement-unit.h"

#include "constants.h"

#include <vector>
#include <utility>

namespace rcr {
namespace geovis {

Adafruit_BNO055 InertialMeasurementUnit::bno_;

InertialMeasurementUnit::InertialMeasurementUnit()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR) {
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
  return bno_.begin();
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
  auto orientation = SampleForMeanVector<2>(Adafruit_BNO055::VECTOR_EULER);
  auto linear = SampleForMeanVector<2>(Adafruit_BNO055::VECTOR_LINEARACCEL);
  auto gravity = SampleForMeanVector<2>(Adafruit_BNO055::VECTOR_GRAVITY);

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

} // namespace level1_payload
} // namespace rcr
