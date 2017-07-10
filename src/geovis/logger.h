#ifndef RCR_GEOVIS_LOGGER_H_
#define RCR_GEOVIS_LOGGER_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "initializable.h"

#include <SdFat.h>

namespace rcr {
namespace geovis {

class Logger : public virtual Initializable {
 public:
  Logger();

  bool Init();

  // Open a new file for writing.
  bool Open(String path);

  // Write text to the end of the file.
  void Write(const String& text);

  // Flush the write buffer.
  void Flush();

  // Close the currently open file forever. Returns false if no file is open.
  bool Close();

  ~Logger();

  //static constexpr int chipSelect = BUILTIN_SDCARD;

 protected:
  static File file_; // File I/O manager
  
  static SdFatSdio sd_card_; // SD card I/O manager
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_LOGGER_H_
