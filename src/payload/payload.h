#ifndef RCR_LEVEL1PAYLOAD_PAYLOAD_H_
#define RCR_LEVEL1PAYLOAD_PAYLOAD_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


// SD card libraries
#include <lib/SdFat/src/SdFat.h>
#include <lib/SdFat/src/SdFatConfig.h>
#include <lib/SdFat/src/BlockDriver.h>
#include <lib/SdFat/src/MinimumSerial.h>
#include <lib/SdFat/src/SysCall.h>
#include <lib/SdFat/src/FreeStack.h> // something is wrong with this library.

// Sensor libraries.
#include <lib/sensors/Adafruit_Sensor/Adafruit_Sensor.h>
#include <lib/sensors/Adafruit_GPS/Adafruit_GPS.h>
#include <lib/sensors/Adafruit_BNO055/Adafruit_BNO055.h>
#include <lib/sensors/Adafruit_BME280_Library/Adafruit_BME280.h>

#include <lib/i2c_t3/i2c_t3.h> // I2C for teensy (replaces wire.h)

// RCR headers
#include "setup-object.h"

namespace rcr {
namespace level1payload {

SdFatSdio sd_card; // SD card manager
Adafruit_BME280 bme; // I2C connection to BME280

// Array of pointers to Setupable-implementing objects which require 
// initialization logic to take place in setup() function.
// For more info, see "setupable.h".
//Setupable* setupables[] = {  };

void print_setup_message(void) {
  Serial.print("In setup");
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.println(".");
  Serial.println();
  delay(512);
}

void setup_objects() {
  setup_object<SdFatSdio>(sd_card, 
    "ERROR: SD card could not be found or setup.", "Success: SD card ready.");
  setup_object<Adafruit_BME280>(bme,
    "ERROR: BME280 sensor could not be found or setup.", "Success: BME280 ready.");
}

inline void setup() {
  // Illuminate LED.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(3000);
  
  // Start serial communication. 
  Serial.begin(9600); // bits/second does not matter for Teensy 3.6
  print_setup_message();

  // Setup objects / verify working condition.
  setup_objects();

  Serial.println();
  Serial.println("SETUP COMPLETE.");
  Serial.println();
} // setup()

void write_to_sd() {
  auto log_path = "nolan-test.log";
  sd_card.remove(log_path); // remove file from last time.

  File file = sd_card.open(log_path, FILE_WRITE); // Open a new file for writing.
  
  // Initiate file for writing.
  if(!file) {
    Serial.println("File could not be initialized."); 
  }
  else {
    file.println("Writing this to file.");
    file.close(); // Close when finished.
  }
}

void printBmeData() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" �C");

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

bool written = false;

inline void loop() {
  printBmeData();
  if (!written) {
    write_to_sd();
    written = true;
  }
  delay(1000);
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_PAYLOAD_H_
