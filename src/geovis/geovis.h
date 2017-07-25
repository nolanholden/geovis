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

  inline void blink() {
    // Illuminate LED.
    digitalWrite(kLedPin, HIGH);

    // Ensure gps reciever is encoding.
    gps_receiver.smartDelay();

    // Dim LED.
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
}


namespace {
  auto user_quit = 1u; // 'quit' instruction flag. (see while() below)
} // namespace

// Begin GEOVIS flight-path logging. Continue forever until human intervention.
inline void loop() {
  auto num_prints_backward_counter = 512u;
  String csv_line = "";

  Serial.println("Entering flight loop.");

  while (user_quit) { // ARM reccomends while(1u) for greatest efficiency
    csv_line = ""; // Set empty each time.

    // Get a line of data.
    // - Time
    csv_line += millis();
    csv_line += ",";

    // - Sensor data
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

    // Ensure we are always receiving gps data.
    gps_receiver.smartDelay(); // no delay.

    blink();

    // Allow user to quit by 'q'.   // TODO: terminate via radio instruction
    if (Serial.available() > 0 && Serial.read() == 'q') {
      --user_quit;
      geovis_util::clear_serial_input();
    }
  }
}

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVIS_H_
