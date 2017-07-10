#ifndef RCR_GEOVIS_GEOVISUTIL_H_
#define RCR_GEOVIS_GEOVISUTIL_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace geovis {

// Empty the input serial. Do not do this often.
inline void clear_serial_input() {
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.flush();
  Serial.clear();
}

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVISUTIL_H_
