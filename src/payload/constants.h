namespace rcr {
namespace level1payload {
  
// TODO: arrange for runtime filename-resolution. (likely use millis())
// File path name:
constexpr const char* kLogPath = "flight.log";

// Main loop delay (milliseconds)
constexpr const uint32_t kLoopDelay = 1024;

// Kalman constants:
constexpr float KALMAN_PROCESS_NOISE = 0.01;
constexpr float KALMAN_MEASUREMENT_NOISE = 0.25;
constexpr float KALMAN_ERROR = 1.0f;

} // namespace level1_payload
} // namespace rcr
