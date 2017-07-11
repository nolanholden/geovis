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

class Logger : public virtual Initializable {
 public:
  Logger(const char* desired_path);

  bool Init();

  // Write text to the end of the file.
  bool WriteLine(const String& text);

  ~Logger();

  //static constexpr int chipSelect = BUILTIN_SDCARD;

 protected:
  bool WriteLineWithPath(const String& text, const char* path);

  File file_; // File I/O manager
  
  const char* path_;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_LOGGER_H_
