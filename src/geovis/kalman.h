#ifndef RCR_GEOVIS_KALMAN_H_
#define RCR_GEOVIS_KALMAN_H_

namespace rcr {
namespace geovis {

struct kalman_t {
  double process_noise;     // process noise covariance
  double measurement_noise; // measurement noise covariance
  double value;             // value
  double error;             // estimation error covariance
  double gain;              // kalman gain
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_KALMAN_H_
