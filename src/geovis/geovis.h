#ifndef RCR_GEOVIS_GEOVIS_H_
#define RCR_GEOVIS_GEOVIS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// RCR headers
#include "atmospheric-sensor.h"
#include "constants.h"
#include "gps-receiver.h"
#include "inertial-measurement-unit.h"
#include "logger.h"
#include "novelty-printouts.h"

namespace rcr {
namespace geovis {

Logger logger;

// Sensors:
AtmosphericSensor atmospheric_sensor; // Barometer/Thermometer/Hygometer
GpsReceiver gps_receiver;                      // GPS module
InertialMeasurementUnit imu;          // IMU

inline void blink() {
  // Illuminate LED.
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  gps_receiver.smartDelay(kLoopDelay);
}

// Initialize objects.
inline void initialize_objects() {
  // SD card
  if (!logger.Init())
    Serial.println("SD card initialization failed.");

  // GPS sensor
  if (!gps_receiver.Init())
    Serial.println("GPS initialization failed.");

  // IMU
  if (!imu.Init())
    Serial.println("IMU initialization failed.");

  // Barometer/Thermometer/Hygometer
  if (!atmospheric_sensor.Init())
    Serial.println("Atmospheric sensor initialization failed.");
}


inline void setup() {
  blink();

  // Start serial communication.
  Serial.begin(9600); // baud does not matter for Teensy 3.6
  Serial.println("In setup.");

  // Initialize objects.
  initialize_objects();

  // Initialize output file(s).
  {
    logger.Open(String{ millis() });
    String csv_header = "";
    csv_header += atmospheric_sensor.kCsvHeader;
    csv_header += gps_receiver.kCsvHeader;
    csv_header += imu.kCsvHeader;
    logger.WriteLine(csv_header);
  }

  Serial.println("Setup complete.");
}

inline void loop() {
  while (Serial1.available() > 0) {
    gps_receiver.gps_.encode(Serial1.read());
  }
  // Testing items
  Serial.println(gps_receiver.GetCsvLine());
  gps_receiver.smartDelay(200ul);

  //// Weather
  //Serial.println(atmospheric_sensor.kCsvHeader);
  //Serial.println(atmospheric_sensor.GetCsvLine());

  //// GPS
  //Serial.println(gps_receiver.kCsvHeader);
  //Serial.println(gps_receiver.GetCsvLine());

  //// IMU
  //Serial.println(imu.kCsvHeader);
  //Serial.println(imu.GetCsvLine());



  // Get a line of data.
  // ...

  // Print it to the file(s).
  //logger.WriteLine(String{ line });
  //write_to_sd(kLogPath, line);

  // Wait a moment.
  blink();
}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_GEOVIS_GEOVIS_H_
