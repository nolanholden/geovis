#include "logger.h"

// SD card library
#include <SdFat.h>
#include "debug-settings.h"

namespace rcr {
namespace geovis {

File Logger::file_;
SdFatSdio Logger::sd_card_;


Logger::Logger(const char* path) : path_(path) {}

bool Logger::Init() {
  return sd_card_.begin();
}

bool Logger::Write(const String& text) {
  // Open a (new/existing) file for writing.
  file_ = sd_card_.open(path_, FILE_WRITE);

  // Write to file (if able).
  if (file_) {
    file_.println(text);
    file_.close(); // Close when finished.
    return true;
  }
#if DEBUG_LOGGER
  Serial.println("File could not be initialized.");
#endif
  return false;
}

Logger::~Logger() {}

} // namespace geovis
} // namespace rcr
