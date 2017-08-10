#ifndef RCR_GEOVIS_CONSTANTS_H_
#define RCR_GEOVIS_CONSTANTS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace geovis {

// GlobalTop GPS commands. See below:
//  - https://cdn-shop.adafruit.com/datasheets/GlobalTop-FGPMMOPA6H-Datasheet-V0A.pdf
//  - https://github.com/adafruit/Adafruit_GPS/blob/master/Adafruit_GPS.h
constexpr const char* const PMTK_SET_NMEA_OUTPUT_RMCGGA = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28";
constexpr const char* const PMTK_SET_NMEA_UPDATE_5HZ    = "$PMTK220,200*2C";
constexpr const char* const PMTK_API_SET_FIX_CTL_5HZ    = "$PMTK300,200,0,0,0,0*2F";

// GPS wiring & data rate.
constexpr uint8_t GPS_RX_PIN = 9; // Note: GPS module's TX connects to this pin.
constexpr uint8_t GPS_TX_PIN = 10; // Note: GPS module's RX connects to this pin.
constexpr uint32_t GPS_BAUD = 9600; // Note: All testing with Teensy 3.6 suggests
// that baud rates other than 9600 are incompatible. However, 9600 is ok for
// 5 Hz GPS refresh (which itself is very sufficient.)


// Kalman constants:
constexpr float KALMAN_PROCESS_NOISE = 0.01f;
constexpr float KALMAN_MEASUREMENT_NOISE = 0.25f;
constexpr float KALMAN_ERROR = 1.f;

// LED Pin
constexpr uint8_t kLedPin = (uint8_t)13;

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_CONSTANTS_H_
