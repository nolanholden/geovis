#ifndef RCR_LEVEL1PAYLOAD_SETUPABLE_H_
#define RCR_LEVEL1PAYLOAD_SETUPABLE_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace level1payload {

// An interface indicating a class requires construction (or constructor-like)
// logic to be delayed until Arduino setup() function.
class Setupable {
 public:
  // Setup this object. (Especially within the setup() function in the 
  // Arduino program.)
  virtual void Setup() = 0;

  // Must provide virtual d'tor and body to prevent memory leak.
  virtual ~Setupable() {}
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_SETUPABLE_H_
