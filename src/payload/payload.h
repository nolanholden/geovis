#ifndef RCR_LEVEL1PAYLOAD_PAYLOAD_H_
#define RCR_LEVEL1PAYLOAD_PAYLOAD_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <string>
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
#include "setup-object.h"
#include "sampling.h" // Inertial Measurement Unit vector sampling utilities

namespace rcr {
namespace level1payload {

// TODO: setup runtime filename resolution. (likely use millis())
// File path names
static constexpr const char* kGpsLogPath = "gps.log";
static constexpr const char* kOrientationLogPath = "imu.log";
static constexpr const char* kBarometricLogPath = "baro.log";

// Main loop delay (milliseconds)
static constexpr const uint32_t kLoopDelay = 1024;


// File I/O
File file; // File manager
SdFatSdio sd_card; // SD card I/O manager

// Sensors
Adafruit_GPS gps(&Serial1); // GPS sensor
Adafruit_BNO055 bno; // BNO055 9-DOF sensor
Adafruit_BME280 bme; // BME280 Barometer (I2C connection)


inline void print_with_ellipses(const char* message) {
  Serial.print(message);
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.println(".");
  Serial.println();
  delay(512);
}

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

// Barometric data (altitude) from BME280.
void append_barometric_data(String& string_to_append) {
  // Temperature (*C)
  string_to_append += bme.readTemperature();
  string_to_append += ",";

  // Ambient pressure (Pascals)
  string_to_append += bme.readPressure();
  string_to_append += ",";

  // Relative humidity (%)
  string_to_append += bme.readHumidity();
  string_to_append += ",";

  // Pressure altitude (meters)
  string_to_append += bme.readAltitude(1013.25f); // 101325 Pa (std pressure)
}

// 9DOF data (inertial / orientation) from BNO055.
void append_inertial_data(String& string_to_append) {

  // Linear acceleration vector.
  auto linear = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  
  // Gravitational accelleration vector.
  auto gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
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

  // Initialize output files.
  Serial.println("Setting up output files...");
  write_to_sd(kBarometricLogPath, "*C, Pa, %, m");
  Serial.println("SUCCESS: files ready.");

  Serial.println();
  Serial.println("SETUP COMPLETE.");
  print_with_ellipses("Starting loop");
} // setup()

String bme_data = "";
inline void loop() {
  bme_data = "";
  append_barometric_data(bme_data);
  write_to_sd(kBarometricLogPath, bme_data);
  delay(kLoopDelay);
}

// PROVISIONAL; REMOVE BEFORE FLIGHT =^.^=
void printBmeData() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  Serial.print("Ambient pressure = ");
  Serial.print(bme.readPressure());
  Serial.println(" Pa");

  Serial.print("Relative humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" percent");

  Serial.print("Pressure altitude = ");
  Serial.print(bme.readAltitude(1013.25f)); // 101325 Pa (i.e., std pressure)
  Serial.println(" meters");
  Serial.println();
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_PAYLOAD_H_
