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
#include "debug-settings.h"
#include "geovis-util.h"
#include "gps-receiver.h"
#include "inertial-measurement-unit.h"
#include "logger.h"
#include "printouts.h"
#include "updateable.h"

namespace rcr {
namespace geovis {

namespace {
  Logger logger("flight.csv");
  InertialMeasurementUnit imu;           // IMU
  GpsReceiver gps_receiver;              // GPS module
  AtmosphericSensor atmospheric_sensor;  // Barometer/Thermometer/Hygometer

  Initializable* components[] = { &logger, &imu, &gps_receiver, &atmospheric_sensor };
  Sensor* sensors[] = { &imu, &gps_receiver, &atmospheric_sensor };
  Updateable* updateables[] = { &gps_receiver };

  inline void blink() {
    digitalWrite(kLedPin, HIGH);
    digitalWrite(kLedPin, LOW);
  }
} // namespace

auto num_prints_backward_counter = 1028u;
String csv_line = "";

inline void setup() {
  // Wait a moment.
  delay(1024u);

  // Set LED pin.
  pinMode(kLedPin, OUTPUT);

  // Start serial communication.
  Serial.begin(9600l); // baud does not matter for Teensy 3.6

  // Initialize components.
  for (auto& c : components) {
    if (!c->Init()) {
      // Print any failures
      Serial.print("Initialization failed for: ");
      Serial.println(c->display_name());
    }
  }

  {
    // Initialize log file with header for each comma-delimited value.
    String csv_header = "millis,"; // first column is time
    for (auto& s : sensors) {
      csv_header += s->kCsvHeader;
    }

    // Write it only once.
    if (!logger.WriteLine(csv_header)) {
      Serial.println("Output file did not initialize.");
    }
    else {
      Serial.println("Output file intitialized.");
    }
  }

  Serial.println("Setup complete.");
  delay(3000);
  Serial.println("Entering flight loop.");
}

// Begin GEOVIS flight-path logging. Continue forever until human intervention.
inline void loop() {
  for (auto& u : updateables) {
    u->Update();
  }

  // Get a line of data.
  csv_line = ""; // Set empty each time.
  csv_line += millis();
  csv_line += ",";
  for (auto& s : sensors) {
    csv_line += s->GetCsvLine();
  }

  // Print the line to the file. Display proof only at first.
  if (num_prints_backward_counter) {
    --num_prints_backward_counter;
    Serial.println(csv_line);
    if (logger.WriteLine(csv_line)) {
      Serial.println("Successful print.");
    }
    else {
      Serial.println("Print FAILED.");
    }
  }
  else {
    logger.WriteLine(csv_line);
  }

  blink();
}

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVIS_H_
