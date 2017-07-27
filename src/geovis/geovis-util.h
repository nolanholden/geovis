#ifndef RCR_GEOVIS_GEOVISUTIL_H_
#define RCR_GEOVIS_GEOVISUTIL_H_

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include "arduino.h"
//#else
//	#include "WProgram.h"
//#endif

//#include "constants.h"
//#include "initializable.h"

//#include <vector>

namespace rcr {
namespace geovis {
namespace geovis_util {

// Absolute value.
#undef abs
template <typename T>
inline constexpr T abs(T arg) { return static_cast<T>(0) ? -arg : arg; }

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

// Express a fatal error by locking the main thread and blinking morse code SOS.
void illuminate_morse_code_sos() {
  digitalWrite(kLedPin, LOW);
  // Times in milliseconds.
  auto dot = 64u;
  auto dash = dot * 3;
  auto pause_between_elements = dot;
  auto pause_between_characters = dot * 3;
  auto pause_between_words = dot * 7;

  delay(pause_between_words);

  // 'S'
  delay(pause_between_characters);
  for (auto i = 0; i < 3; ++i) {
    if (i != 0) {
      delay(pause_between_elements);
    }
    digitalWrite(kLedPin, HIGH);
    delay(dot);
    digitalWrite(kLedPin, LOW);
  }

  // 'O'
  delay(pause_between_characters);
  for (auto i = 0; i < 3; ++i) {
    if (i != 0) {
      delay(pause_between_elements);
    }
    digitalWrite(kLedPin, HIGH);
    delay(dash);
    digitalWrite(kLedPin, LOW);
  }

  // 'S'
  delay(pause_between_characters);
  for (auto i = 0; i < 3; ++i) {
    if (i != 0) {
      delay(pause_between_elements);
    }
    digitalWrite(kLedPin, HIGH);
    delay(dot);
    digitalWrite(kLedPin, LOW);
  }

  delay(pause_between_characters);
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
