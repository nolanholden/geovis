#ifndef RCR_LEVEL1PAYLOAD_KALMAN_H_
#define RCR_LEVEL1PAYLOAD_KALMAN_H_

namespace rcr {
namespace level1payload {

struct kalman_t {
  float process_noise;     // process noise covariance
  float measurement_noise; // measurement noise covariance
  float value;             // value
  float error;             // estimation error covariance
  float gain;              // kalman gain
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_KALMAN_H_
