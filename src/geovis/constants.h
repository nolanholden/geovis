namespace rcr {
namespace geovis {

constexpr uint8_t kLedPin = (uint8_t)0;

// Main loop delay (milliseconds)
constexpr const unsigned long kLoopDelay = 256ul;

// Kalman constants:
constexpr float KALMAN_PROCESS_NOISE = 0.01f;
constexpr float KALMAN_MEASUREMENT_NOISE = 0.25f;
constexpr float KALMAN_ERROR = 1.f;

} // namespace level1_payload
} // namespace rcr
