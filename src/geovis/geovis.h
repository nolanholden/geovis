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

#include <vector>

namespace rcr {
namespace geovis {

namespace {
  Logger logger("flight.csv");
  InertialMeasurementUnit imu;           // IMU
  GpsReceiver gps_receiver;              // GPS module
  AtmosphericSensor atmospheric_sensor;  // Barometer/Thermometer/Hygometer

  std::vector<Initializable*> components;
  // components{ &logger, &imu, &gps_receiver, &atmospheric_sensor }; confuses
  // static analysis.
  std::vector<Sensor*> sensors{ &imu, &gps_receiver, &atmospheric_sensor };
  std::vector<Updateable*> updateables;

  inline void blink() {
    digitalWrite(kLedPin, HIGH);
    digitalWrite(kLedPin, LOW);
  }

  // Print any failures
  inline void initialize_components() {
    components.push_back(&logger);
    components.push_back(&imu);
    components.push_back(&gps_receiver);
    components.push_back(&atmospheric_sensor);

    for (auto& c : components) {
      if (!c->Init()) {
        Serial.print("Initialization failed for: ");
        Serial.println(c->display_name());
      }
    }
  }

} // namespace

inline void setup() {
  // Wait a moment.
  delay(1024u);

  // Set LED pin.
  pinMode(kLedPin, OUTPUT);

  // Start serial communication.
  Serial.begin(9600l); // baud does not matter for Teensy 3.6

  // Initialize objects.
  initialize_components();

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
  Serial.println("Entering flight loop.");
}


namespace {
  auto num_prints_backward_counter = 512u;
  String csv_line = "";
} // namespace

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
  if (1/*num_prints_backward_counter*/) {
    --num_prints_backward_counter;
    Serial.println(csv_line);
    if (logger.WriteLine(csv_line)) {
      Serial.println("Successful print.");
    }
    else {
      geovis_util::illuminate_morse_code_sos();
      Serial.println("Print FAILED.");
    }
  }
  else {
    if (!logger.WriteLine(csv_line)) {
      geovis_util::illuminate_morse_code_sos();
    }
  }

  blink();
}

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVIS_H_
