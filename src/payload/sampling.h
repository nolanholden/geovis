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
// Gets the mean x,y,z values of the given IMU vector for a given number of samples.
template <int SampleSize>
imu::Vector<3> sample_imu(
    Adafruit_BNO055& bno,
    Adafruit_BNO055::adafruit_vector_type_t vector_type) {
  // Resultant vector of arithmetic-mean x,y,z values.
  auto mean_vector = imu::Vector<3>();
  
  // Ensure x,y,z == 0.0
  mean_vector.x() = 0.0;
  mean_vector.y() = 0.0;
  mean_vector.z() = 0.0;

  // Sample the IMU sensor.
  auto imu_vectors = std::vector<imu::Vector<3>>(static_cast<size_t>(SampleSize));
  for (auto i = 0; i < SampleSize; ++i) {
    imu_vectors.push_back(
      std::move(bno.getVector(vector_type))
    );
  }

  // Compute the mean.
  auto sample_size_double = static_cast<double>(SampleSize);
  for (auto& v : imu_vectors) {
    //Serial.println(v.x());
    mean_vector.x() += v.x();
    mean_vector.y() += v.y();
    mean_vector.z() += v.z();
  }
  mean_vector.x() /= sample_size_double;
  mean_vector.y() /= sample_size_double;
  mean_vector.z() /= sample_size_double;
  
  //Serial.println(mean_vector.x());
  return mean_vector;
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_SAMPLING_H_
