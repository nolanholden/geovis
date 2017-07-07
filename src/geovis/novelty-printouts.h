#ifndef RCR_LEVEL1PAYLOAD_NOVELTYPRINTOUTS_H_
#define RCR_LEVEL1PAYLOAD_NOVELTYPRINTOUTS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace geovis {

// Print message with a delayed, trailing ellipses.
template <typename T>
inline void print_with_ellipses(T message) {
  Serial.print(message);
  delay(512);
  Serial.print(" .");
  delay(512);
  Serial.print(" .");
  delay(512);
  Serial.println(" .");
  delay(512);
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_NOVELTYPRINTOUTS_H_
