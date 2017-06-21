#include "payload.h" // C++ file which acts as a surrogate this .ino program.

// Must include libraries here for use in headers.

// Sensors:
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_GPS.h>

// SD I/O:
#include <BlockDriver.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SysCall.h>
//#include <FreeStack.h> // something is wrong with this library.


void setup() {
  rcr::level1payload::setup();
}

void loop() {
  rcr::level1payload::loop();
}

