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
#include "bme.h"
#include "constants.h"
#include "data-acquisition-interface.h"
#include "gps.h"
#include "novelty-printouts.h"
#include "setup-object.h"

namespace rcr {
namespace level1payload {

// File I/O
File file; // File I/O manager
SdFatSdio sd_card; // SD card I/O manager

// Sensors
//Adafruit_GPS gps(&Serial1); // GPS sensor
//Adafruit_BNO055 bno; // BNO055 9-DOF sensor
//Adafruit_BME280 bme; // BME280 Barometer (I2C connection)
Bme bme;
Gps gps;

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
  Serial.println(gps.Init() ? "GPS OK" : "GPS init failed.");
  //setup_object<Adafruit_GPS>(gps, uint32_t{9600}, "GPS sensor");
  //gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // request RMC & GGA
  //gps.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  //gps.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);

  // IMU
  //setup_object<Adafruit_BNO055>(bno, "BNO055");
  Adafruit_BME280
  // Barometer & hygrometer
  //setup_object<Adafruit_BME280>(bme, "BME280");
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
  bme.Init();

  // Initialize output file(s).
  Serial.println("Setting up output files...");
  {
    String csv_header = "";
    csv_header += kBarometricCsvHeader;
    csv_header += kImuCsvHeader;
    csv_header += kGpsCsvHeader;
    write_to_sd(kLogPath, csv_header);
  }

  Serial.println();
  Serial.println("SETUP COMPLETE.");
  print_with_ellipses("Starting loop");
} // setup()

// Line of csv data.
String line = "";

inline void loop() {
  // Get a line of data.
  //line = "";
  //append_barometric_data(bme, line);
  //Serial.println(kBarometricCsvHeader);
  //Serial.println(line);
  //line = "";
  //append_inertial_data(bno, line);
  //Serial.println(kImuCsvHeader);
  //Serial.println(line);

  Serial.println(gps.getSpeed());
  Serial1.println(gps.getLatitude());
  Serial1.println(gps.getAltitude());
  line = "";
  append_gps_data(gps, line);
  Serial.println(kGpsCsvHeader);
  Serial.println(line);

  // Print it to the file(s).
  //write_to_sd(kLogPath, line);

  // Wait a moment.
  delay(512);
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_PAYLOAD_H_
