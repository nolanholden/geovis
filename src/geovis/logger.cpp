#include "logger.h"

// SD card libraries
#include <BlockDriver.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SysCall.h>
//#include <FreeStack.h> // something is wrong with this library.

namespace rcr {
namespace geovis {

File Logger::file_;
SdFatSdio Logger::sd_card_;


Logger::Logger() {}

bool Logger::Init() {
  return sd_card_.begin();
}

bool Logger::Open(String path) {
  // Open a new file for writing.
  file_ = sd_card_.open(path, FILE_WRITE);

  if (file_) {
    return true;
  } // else, swallow the error.
  return false;
}

void Logger::Write(const String& text) {
  file_.print(text);
}

void Logger::WriteLine(const String& text) {
  file_.println(text);
}

void Logger::Close() {
  file_.close();
}

} // namespace level1_payload
} // namespace rcr
