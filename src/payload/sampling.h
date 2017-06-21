#ifndef RCR_LEVEL1PAYLOAD_SAMPLING_H_
#define RCR_LEVEL1PAYLOAD_SAMPLING_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace level1payload {

// TODO: Is possible overflow an issue?
// Sample the IMU sensor a given number of times and return the mean.
template <size_t SampleSize>
imu::Vector<3> sample_vector(
    imu::Vector<3> (*get_vector_func)(Adafruit_BNO055::adafruit_vector_type_t),
    Adafruit_BNO055::adafruit_vector_type_t vector_type,
    size_t sample_size = 1) {
  // Resultant vector of arithmetic-mean x,y,z values.
  auto mean_vector = imu::Vector<3>();
  
  // Ensure x,y,z == 0.0
  mean_vector.x() = mean_vector.y() = mean_vector.z() = 0.0;

  // Sample the IMU sensor.
  auto imu_vectors = std::vector<imu::Vector<3>>(SampleSize);
  for (auto i = 0; i < SampleSize; ++i) {
    imu_vectors.push_back(
      std::move(get_vector_func(vector_type))
    );
  }

  // Compute the mean.
  auto sample_size_double = static_cast<double>(sample_size);
  for (auto& v : imu_vectors) {
    mean_vector.x() += v.x() / sample_size_double;
    mean_vector.y() += v.y() / sample_size_double;
    mean_vector.z() += v.z() / sample_size_double;
  }

  return mean_vector;
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_SAMPLING_H_
