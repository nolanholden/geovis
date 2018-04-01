#include "geovis-util.h"

#include "constants.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace geovis {
namespace geovis_util {

// Express a fatal error by locking the main thread and blinking morse code SOS.
// Specifiy dot length or use defaults. (All time intervals are defined in
// terms of dot time interval.)
void illuminate_morse_code_sos(const uint32_t dot_milliseconds) {
  digitalWrite(kLedPin, LOW); // start dimmed.

  // Times in milliseconds.
  const auto dash = dot_milliseconds * 3;
  const auto pause_between_elements = dot_milliseconds;
  const auto pause_between_characters = dot_milliseconds * 3;
  const auto pause_between_words = dot_milliseconds * 7;

  delay(pause_between_words);

  // 'S'
  delay(pause_between_characters);
  for (auto i = 0; i < 3; ++i) {
    if (i != 0) {
      delay(pause_between_elements);
    }
    digitalWrite(kLedPin, HIGH);
    delay(dot_milliseconds);
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
    delay(dot_milliseconds);
    digitalWrite(kLedPin, LOW);
  }

  delay(pause_between_characters);
}

// Empty the input serial. Do not do this often.
void clear_serial_input() {
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.flush();
  Serial.clear();
}

} // namespace geovis_util
} // namespace geovis
} // namespace rcr
