#include <Wire.h>

// RCR headers
#include "Setupable.h"
#include "Bmp180.h"
#include "sdcard.h"

SdFatSdio sd_card; // SD card manager
File file; // file manager

// To shorten RCR namespace, use namespace alias "custom".
namespace custom = rcr::level1payload;

// Array of pointers to Setupable-implementing objects which require 
// initialization logic to take place in setup() function.
// For more info, see "setupable.h".
//custom::Setupable* setupables[] = {  };

// Setup MicroSD card I/O. Trap the thread if no card is found.
void setup_sd_card(void) {
  if (!sd_card.begin()) {
    Serial.println("ERROR: SD card could not be found.");
    while (1) { /* Trap the thread. */ }
  }
  Serial.println("SdCard successfully setup.");
  Serial.println();
}

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
  delay(512);

  // Setup objects and verify working condition.
  setup_sd_card();
  
  // Setup() the custom data structures.
  for (custom::Setupable* obj : setupables) {
    obj->Setup();
  }

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
    file.println("I'm text in a file!");
    file.close(); // Close when finished.
  }
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

bool written = false;

void loop() {
  printBmpData();
  if (!written) {
    write_to_sd();
    written = true;
  }
  delay(1000);
}

