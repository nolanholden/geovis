#include "sensor.h"

namespace rcr {
namespace geovis {

Sensor::Sensor(double process_noise, double measurement_noise, double error,
    const char* display_name, const char* csv_header) : kCsvHeader(csv_header) {
  processNoise_ = process_noise;
  measurementNoise_ = measurement_noise;
  error_ = error;
  display_name_ = display_name;
}

kalman_t Sensor::kalmanInit(double initialValue) {
  kalman_t kalman;

  kalman.process_noise = processNoise_;
  kalman.measurement_noise = measurementNoise_;
  kalman.error = error_;
  kalman.value = initialValue;

  return kalman;
}

void Sensor::kalmanUpdate(kalman_t* state, double measurement) {
  // Prediction update
  state->error += state->process_noise;

  // Measurement update
  state->gain = state->error / (state->error + state->measurement_noise);
  state->value = state->value + state->gain * (measurement - state->value);
  state->error = (1 - state->gain) * state->error;
}

} // namespace geovis
} // namespace rcr
