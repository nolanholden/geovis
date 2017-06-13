#ifndef RCR_LEVEL1PAYLOAD_SETUP_H_
#define RCR_LEVEL1PAYLOAD_SETUP_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace level1payload {

// Setup the object. Swallow any errors.
template<typename T>
void setup(T& obj, const char* error_message, const char* success_message) {
  if (!obj.begin()) {
    Serial.println(error_message);
    // Swallow the error; fault tolerance is required.
  }
  Serial.println(success_message);
  Serial.println();
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_SETUP_H_
