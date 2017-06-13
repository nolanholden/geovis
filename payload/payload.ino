#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>

// SD card libraries
#include <BlockDriver.h>
#include <FreeStack.h> // something is wrong with this library.
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SysCall.h>

// Sensor libraries.
#include <Adafruit_BME280.h>
#include <Adafruit_GPS.h>
//#include <Adafruit_BNO055.h>
#include <Adafruit_BME280.h>

// RCR headers
#include "Setupable.h"

FILE file; // file manager
SdFatSdio sd_card; // SD card manager
Adafruit_BME280 bme; // I2C connection to BME280


// To shorten RCR namespace, use namespace alias "custom".
namespace custom = rcr::level1payload;

// Array of pointers to Setupable-implementing objects which require 
// initialization logic to take place in setup() function.
// For more info, see "setupable.h".
//custom::Setupable* setupables[] = {  };

// Setup the object. Swallow any errors.
template<typename T>
void setup_general(T obj, char* error_message, char* success_message) {
  if (!obj.begin()) {
    Serial.println(error_message);
    // Swallow the error; fault tolerance is required.
  }
  Serial.println(success_message);
  Serial.println();
}

void setup_objects(void) {
  setup_general<SdFatSdio>(sd_card, "ERROR: SD card could not be found or setup...",  "Success: SD card ready.");
  setup_general<Adafruit_BME280>(bme, "Could not find a valid BME280 sensor...",      "Success: BME280 ready.");
}

void setup() {
  // Illuminate LED.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Give time to open serial monitor.
  delay(3000);
  
  // Start serial communication. 
  Serial.begin(9600); // bits/second does not matter for Teensy 3.6
  Serial.print("In setup");
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.println(".");
  Serial.println();
  delay(512);

  // Setup objects and verify working condition.
  setup_objects();
  
  // Setup() the custom data structures.
  //for (custom::Setupable* obj : setupables) {
  //  obj->Setup();
  //}

  Serial.println();
  Serial.println("SETUP COMPLETE.");
  Serial.println();
}

void write_to_sd(void) {
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

//void printBmpData(void) {
//  Serial.print("Temperature = ");
//  Serial.print(bmp.temperature());
//  Serial.println(" °C");
//
//  Serial.print("Ambient pressure = ");
//  Serial.print(bmp.ambient_pressure());
//  Serial.println(" Pa");
//
//  Serial.print("Pressure altitude = ");
//  Serial.print(bmp.pressure_altitude());
//  Serial.println(" meters");
//  Serial.println();
//}

bool written = false;

void loop() {
  //printBmpData();
  Serial.println("in loop");
  if (!written) {
    write_to_sd();
    written = true;
  }
  delay(1000);
}

