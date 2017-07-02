#ifndef RCR_LEVEL1PAYLOAD_DATAACQUISITIONINTERFACE_H_
#define RCR_LEVEL1PAYLOAD_DATAACQUISITIONINTERFACE_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sampling.h" // Inertial Measurement Unit vector sampling utilities

namespace rcr {
namespace level1payload {

// 9DOF data (inertial & orientation) from BNO055.
static void append_inertial_data(Adafruit_BNO055& bno, String& string_to_append) {
  String line = ""; // TODO: test computational cost of various string implementations here.

  //// Orientation "vector"
  auto euler = sample_imu<3>(bno, Adafruit_BNO055::VECTOR_EULER);
  line += euler.x();
  line += ",";
  line += euler.y();
  line += ",";
  line += euler.z();
  line += ",";

  // Linear acceleration vector
  auto linear = sample_imu<3>(bno, Adafruit_BNO055::VECTOR_LINEARACCEL);
  line += euler.x();
  line += ",";
  line += euler.y();
  line += ",";
  line += euler.z();
  line += ",";

  // Gravitational accelleration vector
  auto gravity = sample_imu<3>(bno, Adafruit_BNO055::VECTOR_GRAVITY);
  line += euler.x();
  line += ",";
  line += euler.y();
  line += ",";
  line += euler.z();
  line += ",";

  string_to_append += line;
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_DATAACQUISITIONINTERFACE_H_
