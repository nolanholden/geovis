#ifndef RCR_GEOVIS_CONSTANTS_H_
#define RCR_GEOVIS_CONSTANTS_H_

namespace rcr {
namespace geovis {

constexpr uint8_t kLedPin = (uint8_t)13;

// Main loop delay (milliseconds)
constexpr const unsigned long kLoopDelay = 256ul;

// Log path
constexpr const char* kLogPath = "flight.csv";

// Kalman constants:
constexpr float KALMAN_PROCESS_NOISE = 0.01f;
constexpr float KALMAN_MEASUREMENT_NOISE = 0.25f;
constexpr float KALMAN_ERROR = 1.f;

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_CONSTANTS_H_
