#ifndef RCR_GEOVIS_CONSTANTS_H_
#define RCR_GEOVIS_CONSTANTS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace geovis {

constexpr uint8_t kLedPin = (uint8_t)13;

// Main loop delay (milliseconds)
constexpr const uint32_t kLoopDelay = 16u;

// Log path
constexpr const char* kLogPath = "flight";

// Kalman constants:
constexpr float KALMAN_PROCESS_NOISE = 0.01f;
constexpr float KALMAN_MEASUREMENT_NOISE = 0.25f;
constexpr float KALMAN_ERROR = 1.f;

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_CONSTANTS_H_
