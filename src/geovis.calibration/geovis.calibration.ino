#include "inertial-measurement-unit.h"

namespace {
  rcr::geovis::InertialMeasurementUnit imu;
} // namespace

void setup() {
  if (imu.Init()) {
    Serial.println("IMU is ready.");
  }
  else {
    Serial.println("IMU initialization FAILED.");
  }
}

void loop() {
  imu.Calibrate();
}
