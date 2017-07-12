#ifndef RCR_GEOVIS_LOGGER_H_
#define RCR_GEOVIS_LOGGER_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "initializable.h"

#include <SD.h>

namespace rcr {
namespace geovis {

static constexpr uint8_t kChipSelect = BUILTIN_SDCARD;

class Logger : public virtual Initializable {
 public:
  Logger(const char* path);

  bool Init();

  // Write text to the end of the file.
  bool Write(const String& text);

  // Write text to the end of the file, followed by newline.
  bool WriteLine(const String& text);

  ~Logger();

 protected:
  // File I/O manager
  File file_;
  
  const char* path_; // (not necessarily const)
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_LOGGER_H_
