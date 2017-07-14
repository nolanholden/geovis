#include "logger.h"

// SD card library
#include <SD.h>
#include <SPI.h> // TODO: REMOVE

#include "debug-settings.h"

namespace rcr {
namespace geovis {

Logger::Logger(const char* path) {
  path_ = path;
  display_name_ = "Logger";
}

bool Logger::Init() {
  if (!SD.begin(kChipSelect)) {
    Serial.println("SD card initialization failed.");
    return false;
  }
  else {
    Serial.println("Wiring is correct and an SD card is present.");
  }

  // Ensure file creation works.
  if (!WriteLine("File successfully initialized.")) {
    return false;
  }

  Serial.print("Output path is: ");
  Serial.println(path_);

  init_result_ = true;
  return init_result_;
}

bool Logger::Write(const String & text) {
  auto success = false;

  // Open a (new/existing) file for writing.
  file_ = SD.open(path_, FILE_WRITE);

  // Write to file (if able).
  if (file_) {
    file_.print(text);
    success = true;
  }
  else {
    Serial.print("Could not write \"");
    Serial.print(text);
    Serial.print("\" to SD at path: ");
    Serial.println(path_);
  }
  file_.close(); // Always close;

  return success;
}

// Best not to repeat logic as we do below, but we cannot compromise performance here.

bool Logger::WriteLine(const String& text) {
  auto success = false;

  // Open a (new/existing) file for writing.
  file_ = SD.open(path_, FILE_WRITE);

  // Write to file if able.
  if (file_) {
    file_.println(text);
    success = true;
  }
  else {
    Serial.print("Could not write \"");
    Serial.print(text);
    Serial.print("\" to SD at path: ");
    Serial.println(path_);
  }
  file_.close(); // Always close.

  return success;
}

Logger::~Logger() {}

} // namespace geovis
} // namespace rcr
