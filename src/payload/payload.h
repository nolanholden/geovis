#ifndef RCR_LEVEL1PAYLOAD_PAYLOAD_H_
#define RCR_LEVEL1PAYLOAD_PAYLOAD_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <string>
#include <memory>

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
#include "atmospheric-sensor.h"
#include "constants.h"
#include "gps.h"
#include "inertial-measurement-unit.h"
#include "novelty-printouts.h"
#include "setup-object.h"

namespace rcr {
namespace level1payload {

// Sensors:
AtmosphericSensor atmospheric_sensor; // Barometer/Thermometer/Hygometer
Gps gps;                              // GPS module
InertialMeasurementUnit imu;

// File I/O:
File file; // File I/O manager
SdFatSdio sd_card; // SD card I/O manager


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
  // SD card
  setup_object<SdFatSdio>(sd_card, "SD card");

  // GPS sensor
  if (!gps.Init())
    Serial.println("GPS initialization failed.");

  // IMU
  if (!imu.Init())
    Serial.println("IMU initialization failed.");

  // Barometer/Thermometer/Hygometer
  if (!atmospheric_sensor.Init())
    Serial.println("GPS initialization failed.");
}

inline void setup() {
  // Illuminate LED.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(3000);

  // Start serial communication.
  Serial.begin(9600); // bits/second does not matter for Teensy 3.6
  print_with_ellipses("In setup");

  // Initialize DAQ objects.
  setup_objects();

  // Initialize output file(s).
  Serial.println("Setting up output files...");
  {
    String csv_header = "";
    csv_header += atmospheric_sensor.kCsvHeader;
    csv_header += kImuCsvHeader;
    csv_header += gps.kCsvHeader;
    write_to_sd(kLogPath, csv_header);
  }

  Serial.println();
  print_with_ellipses("SETUP COMPLETE");
} // setup()

// Line of csv data.
String line = "";

inline void loop() {
  // Testing items
  Serial.println(gps.getSpeed());
  Serial1.println(gps.getLatitude());
  Serial1.println(gps.getAltitude());

  line = "";
  atmospheric_sensor.GetCsvLine(&line);
  Serial.println(atmospheric_sensor.kCsvHeader);
  Serial.println(line);

  line = "";
  imu.GetCsvLine(&line);
  Serial.println(imu.kCsvHeader);
  Serial.println(line);

  line = "";
  gps.GetCsvLine(&line);
  Serial.println(gps.kCsvHeader);
  Serial.println(line);



  // Get a line of data.
  // ...

  // Print it to the file(s).
  //write_to_sd(kLogPath, line);

  // Wait a moment.
  delay(512);
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_PAYLOAD_H_
