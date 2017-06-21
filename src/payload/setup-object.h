#ifndef RCR_LEVEL1PAYLOAD_SETUPOBJECTS_H_
#define RCR_LEVEL1PAYLOAD_SETUPOBJECTS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace level1payload {

// Setup the object. Swallow any errors.
template<typename T, typename TArg>
inline void setup_object(T& obj, TArg arg, const char* display_name) {
  if (!obj.begin(arg)) {
    Serial.print("ERROR: ");
    Serial.print(display_name);
    Serial.println(" could not be found or setup.");

    // Swallow the error. Fault tolerance is required.
  }
  else {
    Serial.print("Success: ");
    Serial.print(display_name);
    Serial.println(" ready.");
  }
}
template<typename T>
inline void setup_object(T& obj, const char* display_name) {
  if (!obj.begin()) {
    Serial.print("ERROR: ");
    Serial.print(display_name);
    Serial.println(" could not be found or setup.");

    // Swallow the error. Fault tolerance is required.
  }
  else {
    Serial.print("Success: ");
    Serial.print(display_name);
    Serial.println(" ready.");
  }
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_SETUPOBJECTS_H_
