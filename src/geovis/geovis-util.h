#ifndef RCR_GEOVIS_GEOVISUTIL_H_
#define RCR_GEOVIS_GEOVISUTIL_H_

namespace rcr {
namespace geovis {
namespace geovis_util {

// Absolute value.
#undef abs // leave undefined.
template <typename T>
inline constexpr T abs(T arg) { return static_cast<T>(0) ? -arg : arg; }

// Express a fatal error by locking the main thread and blinking morse code SOS.
void illuminate_morse_code_sos();

// Empty the input serial. Do not do this often.
void clear_serial_input();

} // namespace geovis_util
} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVISUTIL_H_
