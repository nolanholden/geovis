#ifndef RCR_LEVEL1PAYLOAD_PAYLOAD_H_
#define RCR_LEVEL1PAYLOAD_PAYLOAD_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <string>
#include <sstream>
#include <vector>

// Sensor libraries.
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_GPS.h>

// SD card libraries
#include <BlockDriver.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SysCall.h>
//#include <FreeStack.h> // something is wrong with this library.

//#include <lib/i2c_t3/i2c_t3.h> // I2C for teensy (replaces wire.h)

// RCR headers
#include "data-acquisition-interface.h"
#include "novelty-printouts.h"
#include "sampling.h" // Inertial Measurement Unit vector sampling utilities
#include "setup-object.h"

namespace rcr {
namespace level1payload {

// TODO: setup runtime filename resolution. (likely use millis())
// File path names
static constexpr const char* kBarometricLogPath = "baro.log";
static constexpr const char* kOrientationLogPath = "imu.log";
static constexpr const char* kGpsLogPath = "gps.log";

static constexpr const char* kBarometricCsvHeader = "*C,Pa,%,m,";
static constexpr const char* kOrientationCsvHeader = "x,y,z,Lx,Ly,Lg,Gx,Gy,Gz,";
static constexpr const char* kGpsCsvHeader = "TODO"; // TODO: determine this.


// Main loop delay (milliseconds)
static constexpr const uint32_t kLoopDelay = 1024;


// File I/O
File file; // File manager
SdFatSdio sd_card; // SD card I/O manager

// Sensors
Adafruit_GPS gps(&Serial1); // GPS sensor
Adafruit_BNO055 bno; // BNO055 9-DOF sensor
Adafruit_BME280 bme; // BME280 Barometer (I2C connection)


void write_to_sd(const char* path, const String& content) {
  // Open a (new/existing) file for writing.
  file = sd_card.open(path, FILE_WRITE);

  // Write to file (if able).
  if (!file) {
    Serial.println("File could not be initialized.");
    // Swallow the error.
  }
  else {
    file.println(content);
    file.close(); // Close when finished.
  }
}

// Setup objects / verify working condition.
inline void setup_objects() {
  setup_object<SdFatSdio>(sd_card, "SD card");
  setup_object<Adafruit_GPS>(gps, uint32_t{9600}, "GPS sensor");
  setup_object<Adafruit_BNO055>(bno, "BNO055");
  setup_object<Adafruit_BME280>(bme, "BME280");
}

inline void setup() {
  // Illuminate LED.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(3000);

  // Start serial communication. 
  Serial.begin(9600); // bits/second does not matter for Teensy 3.6
  print_with_ellipses("In setup");

  setup_objects();

  // Initialize output file(s).
  Serial.println("Setting up output files...");
  {
    String csv_header = "";
    csv_header += kBarometricCsvHeader;
    csv_header += kOrientationCsvHeader;
    csv_header += kGpsCsvHeader;
    write_to_sd(kBarometricLogPath, csv_header);
  }
  Serial.println("SUCCESS: files ready.");

  Serial.println();
  Serial.println("SETUP COMPLETE.");
  print_with_ellipses("Starting loop");
} // setup()

// Line of csv data.
String line = "";

inline void loop() {
  // Get a line of data.
  line = "";
  append_barometric_data(bme, line);
  append_inertial_data(bno, line);

  // Print it to the file(s).
  write_to_sd(kBarometricLogPath, line);

  // Wait a moment.
  delay(kLoopDelay);
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_PAYLOAD_H_
