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
AtmosphericSensor atmospheric_sensor;  // Barometer/Thermometer/Hygometer
GpsReceiver gps_receiver;              // GPS module
InertialMeasurementUnit imu;           // IMU

std::vector<Sensor*> sensors{
  &atmospheric_sensor,
  &gps_receiver,
  &imu
};
std::vector<Initializable*> components;
// = { &logger, &atmospheric_sensor, &gps_receiver, &imu }; does not work.

inline void blink() {
  // Illuminate LED.
  digitalWrite(kLedPin, HIGH);
  
  // Ensure we are always receiving gps data.
  gps_receiver.smartDelay(kLoopDelay);
  while (Serial1.available() > 0) {
    gps_receiver.gps_.encode(Serial1.read());
  }

  // Dim LED.
  digitalWrite(kLedPin, LOW);
}

// Print any failures
inline void initialize_components() {
  components.push_back(&logger);
  //components.push_back(&atmospheric_sensor);
  components.push_back(&gps_receiver);
  components.push_back(&imu);

  for (auto& c : components) {
    if (!c->Init()) {
      Serial.println(String{ c->display_name() } + " initialization failed.");
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
    //csv_header += atmospheric_sensor.kCsvHeader; // Atmospheric data header
    csv_header += gps_receiver.kCsvHeader;       // GPS header
    csv_header += imu.kCsvHeader;                // IMU header

    // Write it only once.
    if (!logger.WriteLine(csv_header)) {
      Serial.println("Output file did not initialize.");
    }
  }

  Serial.println("Setup complete.");
}

inline void fly() {
  auto continue_logging = true;
  auto num_Qs = 0; // number of 'quit' instructions. (require more than one)

  String csv_line;
  Serial.println("Entering flight loop.");

  while (continue_logging) {
    csv_line = ""; // Set empty each time.

    // Get a line of data.
    // - Time
    csv_line += millis();
    csv_line += ",";

    // - Sensor data
    //csv_line += atmospheric_sensor.GetCsvLine();
    csv_line += gps_receiver.GetCsvLine();
    csv_line += imu.GetCsvLine();

    // Print the line to the file.
    logger.WriteLine(csv_line);

    // Allow user to quit.   // TODO: terminate via radio instruction
    if (Serial.available() > 0 && Serial.read() == 'q') {
      // Prevent accidents; require many 'q's.
      if (++num_Qs > 2) continue_logging = false;
      clear_serial_input();
    }

    // Wait a moment.
    delay(1024);// blink();
  }
}

bool menu_should_display = true;

// Allow calibration because our IMU's calibration persists only for one power
// cycle (has no on-board EEPROM).
inline void loop() {
  // Show the menu.
  if (menu_should_display) {
    print_menu();
    menu_should_display = false;
  }

  // User decides.
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 'c': {
        Serial.println("Received input [c].");
        print_with_ellipses("Beginning IMU calibration");
        imu.Calibrate();
        menu_should_display = true;
        break;
      }
      case 'r': {
        Serial.println("Rebooting.");
        setup();
        menu_should_display = true;
        break;
      }
      case 'f': {
        Serial.println("Received input [f].");
#if DEBUG_ANY
        fly();
        menu_should_display = true;
#else
        //if (components_fully_initialized(components)) {
          print_with_ellipses("All components initialized. Beginning flight");
          fly();
          menu_should_display = true;
        //}
        //else {
        //  Serial.println("Not all components are initialized:");
        //  express_initialization(components);
        //}
#endif // DEBUG_ANY

        break;
      }
      default: {
        Serial.println("Input not recognized.");
        break;
      }
    }

    // Clear remaining characters.
    clear_serial_input();
    Serial.flush();
    Serial.clear();
  }

  // Wait a moment.
  blink();
}

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GEOVIS_H_
