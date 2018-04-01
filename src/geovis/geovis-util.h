#ifndef RCR_GEOVIS_GEOVISUTIL_H_
#define RCR_GEOVIS_GEOVISUTIL_H_

#if defined(ARDUINO) && ARDUINO >= 100
#  include "arduino.h"
#else
#  include "WProgram.h"
#endif

namespace rcr {
namespace geovis {
namespace geovis_util {

// Absolute value.
#undef abs // leave undefined.
template <typename T>
inline constexpr T abs(T arg) { return arg < static_cast<T>(0) ? -arg : arg; }

// Express a fatal error by locking the main thread and blinking morse code SOS.
void illuminate_morse_code_sos(const uint32_t dot_milliseconds = 64u);

// Empty the input serial. Do not do this often.
void clear_serial_input();

} // namespace geovis_util
} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVISUTIL_H_
