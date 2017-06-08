#include <Wire.h>

// SD card libraries
#include <BlockDriver.h>
//#include <FreeStack.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SysCall.h>

// RCR headers
#include "Setupable.h"
#include "Bmp180.h"

// To shorten RCR namespace, use namespace alias "custom".
namespace custom = rcr::level1payload;

custom::Bmp180 bmp;
File file; // file object
//SdFatSdio sd_card; // MicroSD card

// Array of pointers to Setupable-implementing objects which require 
// initialization logic to take place in setup() function.
// For more info, see "Setupable.h".
custom::Setupable* setupables[] = { &bmp };

void setup() {
  // Illuminate LED.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Give time to open serial monitor.
  delay(2000);
  
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
  
  // Setup() the custom data structures.
  for (custom::Setupable* obj : setupables) {
    obj->Setup();
  }

  Serial.println();
  Serial.println("SETUP COMPLETE.");
  Serial.println();
}

void printBmpData(void) {
  Serial.print("Temperature = ");
  Serial.print(bmp.temperature());
  Serial.println(" °C");

  Serial.print("Ambient pressure = ");
  Serial.print(bmp.ambient_pressure());
  Serial.println(" Pa");

  Serial.print("Pressure altitude = ");
  Serial.print(bmp.pressure_altitude());
  Serial.println(" meters");
  Serial.println();
}

void loop() {
  printBmpData();
  delay(1000);
}

