#ifndef RCR_GEOVIS_GEOVISUTIL_H_
#define RCR_GEOVIS_GEOVISUTIL_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "initializable.h"

#include <vector>

namespace rcr {
namespace geovis {
namespace geovis_util {

bool components_fully_initialized(
    const std::vector<Initializable*>& components) {
  for (auto& component : components) {
    if (!component->IsFullyInitialized()) return false;
  }
  return true;
}

bool express_initialization(
    const std::vector<Initializable*>& components) {
  Serial.println("Component initialization status:");
  for (auto& component : components) {
    Serial.print(component->display_name());
    Serial.println(component->IsFullyInitialized() ? ": true" : ": false");
  }
  return true;
}

// Empty the input serial. Do not do this often.
inline void clear_serial_input() {
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.flush();
  Serial.clear();
}

} // namespace geovis_util
} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVISUTIL_H_
