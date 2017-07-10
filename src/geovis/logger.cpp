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
  if (!sd_card_.begin()) {
    return false;
  }

  { // Ensure file creation works.
    auto success = false;
    auto file = sd_card_.open("anypath.path", FILE_WRITE);
    if (!file) success = true;
    file.close(); // always close
    if (!success) return false;
  }

  // Disallow writing into existing files.
  // Rename the path in this case.
  while (sd_card_.exists(path_)) {
    path_ = String{ path_ + millis() }.c_str();
  }

  init_result_ = true;
  return true;
}

bool Logger::WriteLine(const String& text) {
  auto success = false;

  // Open a (new/existing) file for writing.
  file_ = sd_card_.open(path_, FILE_WRITE);

  // Write to file (if able).
  if (file_) {
    file_.println(text);
    success = true;
  }
#if DEBUG_LOGGER
  Serial.println("File failed to initialized.");
#endif
  file_.close(); // Always close;

  return success;
}

Logger::~Logger() {}

} // namespace geovis
} // namespace rcr
