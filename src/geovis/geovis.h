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
#include "novelty-printouts.h"

namespace rcr {
namespace geovis {

Logger logger("flight.csv");
InertialMeasurementUnit imu;           // IMU
GpsReceiver gps_receiver;              // GPS module
AtmosphericSensor atmospheric_sensor;  // Barometer/Thermometer/Hygometer

std::vector<Sensor*> sensors{ &imu, &gps_receiver, &atmospheric_sensor };
std::vector<Initializable*> components;
// = { &logger, &imu, &gps_receiver, &atmospheric_sensor }; does not work.

inline void blink() {
  // Illuminate LED.
  digitalWrite(kLedPin, HIGH);
  
  // Ensure we are always receiving gps data.
  gps_receiver.smartDelay(kLoopDelay);
  while (Serial2.available() > 0) {
    gps_receiver.gps_.encode(Serial2.read());
  }

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
    Serial.print("Initializing "); Serial.println(c->display_name());
    if (!c->Init()) {
      Serial.print("Initialization failed for: ");
      Serial.println(c->display_name());
    }
  }
}

inline void setup() {
  // Wait a moment.
  delay(1024);

  // Set LED pin.
  pinMode(kLedPin, OUTPUT);

  // Start serial communication.
  Serial.begin(9600); // baud does not matter for Teensy 3.6
  Serial.println("Setting up...");

  // Initialize objects.
  initialize_components();

  {
    // Initialize log file with header for each comma-delimited value.
    String csv_header = "";
    csv_header += "millis,";                     // first column is time
    for (auto& s : sensors) {
      csv_header += s->kCsvHeader;
    }
    csv_header += imu.kCsvHeader;                // IMU header
    csv_header += gps_receiver.kCsvHeader;       // GPS header
    csv_header += atmospheric_sensor.kCsvHeader; // Atmospheric data header


    Serial.println("Initializing output file.");
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

// Begin GEOVIS flight-path logging. Continue forever until human intervention.
inline void fly() {
  auto user_quit = 1u; // 'quit' instruction. (see while() below)
  String csv_line = "";

  Serial.println("Entering flight loop.");

  while (user_quit) { // (ARM reccomends while(1) for greatest efficiency)
    csv_line = ""; // Set empty each time.

    // Get a line of data.
    // - Time
    csv_line += millis();
    csv_line += ",";

    // - Sensor data
    for (auto& s : sensors) {
      csv_line += s->GetCsvLine();
    }

    // Print the line to the file.
    Serial.println(csv_line);
    logger.WriteLine(csv_line);

    // Allow user to quit by 'q'.   // TODO: terminate via radio instruction
    if (Serial.available() > 0 && Serial.read() == 'q') {
      --user_quit;
      geovis_util::clear_serial_input();
    }

    // Wait a moment.
    blink();
  }
}

bool menu_should_display = true;

// Allow calibration because our IMU's calibration persists only for one power
// cycle (has no on-board EEPROM).
inline void loop() {
  // Show the menu.
  if (menu_should_display) {
    printouts::print_menu();
    menu_should_display = false;
  }

  // User decides.
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 'c': {
        Serial.println("Received input [c].");
        printouts::print_with_ellipses("Beginning IMU calibration");
        imu.Calibrate();
        menu_should_display = true;
        break;
      }
      case 'r': {
        Serial.println("Received input [r].");
        printouts::print_with_ellipses("Rebooting");
        setup();
        menu_should_display = true;
        break;
      }
      case 'f': {
        Serial.println("Received input [f].");
        Serial.println("Performing system check.");
        if (geovis_util::components_fully_initialized(components)) {
          printouts::print_with_ellipses(
            "All components initialized. Beginning flight");
          fly();
          menu_should_display = true;
        }
        else {
          Serial.println("Not all components are initialized:");
          geovis_util::express_initialization(components);
        }
        break;
      }
      default: {
        Serial.println("Input not recognized.");
        break;
      }
    }

    // Clear remaining characters.
    geovis_util::clear_serial_input();
    Serial.flush();
    Serial.clear();
  }

  // Wait a moment.
  blink();
}

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVIS_H_
