#include "logger.h"

// SD card library
#include <SdFat.h>
#include "debug-settings.h"

namespace rcr {
namespace geovis {

File Logger::file_;
SdFatSdio Logger::sd_card_;


Logger::Logger(const char* desired_path) : path_(desired_path) {}

bool Logger::Init() {
  if (!sd_card_.begin()) return false;

  // Disallow writing into existing files.
  // Rename the path in this case.
  while (sd_card_.exists(path_)) {
    path_ = (String{ path_ } + String{ millis() }).c_str();
  }

  return true;
}

bool Logger::WriteLine(const String& text) {
  // Open a (new/existing) file for writing.
  file_ = sd_card_.open(path_, FILE_WRITE);

  // Write to file (if able).
  if (file_) {
    file_.println(text);
    file_.close(); // Close when finished.
    return true;
  }
#if DEBUG_LOGGER
  Serial.println("File failed to initialized.");
#endif
  return false;
}

Logger::~Logger() {}

} // namespace geovis
} // namespace rcr
