#ifndef RCR_GEOVIS_KALMAN_H_
#define RCR_GEOVIS_KALMAN_H_

#include <type_traits>

namespace rcr {
namespace geovis {

template <typename RealNumType,
  const RealNumType& process_noise,     // process noise covariance
  const RealNumType& measurement_noise, // measurement noise covariance
  const RealNumType& error>             // estimation error covariance
class Kalman {
  // Allow only floating-point types.
  static_assert(std::is_floating_point<RealNumType>::value,
    "class A can only be instantiated with floating point types");

  // Below: due to GCC bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52036,
  // we cannot use constexpr template args to perform this evaluation (due 
  // to issues with the outdated requirement of external linkage for template
  // arguments.) Therefore... TODO: Fix this upon move to new compliler.

  //// Prevent divide-by-zero.
  //static_assert((process_noise + measurement_noise + error)
  //  != static_cast<RealNumType>(0.),
  //  "The sum of 'process noise covariance', 'measurement noise covariance', and 'estimation error covariance' cannot be zero; this creates a divide-by-zero condition.");

 public:
  // Initialize the Kalman struct to the provided equation constants and
  // initial value.
  Kalman(RealNumType initial_value = static_cast<RealNumType>(1))
    : value_(initial_value), error_(error) {};

  RealNumType value() { return value_; }

  // Update the Kalman filter to reflect the new measurement.
  void Update(RealNumType new_measurement) {
    // Update prediction.
    error_ += process_noise;

    // Update measurement.
    if (error_ + measurement_noise == static_cast<RealNumType>(0.)) {
      // Rescue div-by-zero by setting to original values known not to cause
      // that condition.
      error_ = error + process_noise;
    }
    gain_ = error_ / (error_ + measurement_noise);
    value_ += gain_ * (new_measurement - value_);
    error_ *= (1 - gain_);
  }

 private:
  // filtered value
  RealNumType value_;

  // estimation error covariance
  RealNumType error_;

  // kalman gain
  RealNumType gain_;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_KALMAN_H_
