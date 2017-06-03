#ifndef RCR_LEVEL1PAYLOAD_BMP180_H_
#define RCR_LEVEL1PAYLOAD_BMP180_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_BMP085_Library\Adafruit_BMP085.h>

/* BMP180 setup:
* ----------------------------------------
* Connect BMP180 V-in   to 3.3V or 5.0V
* Connect BMP180 GND    to Ground
* Connect BMP180 SCL    to Analog 5
* Connect BMP180 SDA    to Analog 4
* ----------------------------------------
*/

namespace rcr {
namespace level1payload {

// Encapsulates a BMP180 sensor and providing selected access to its interface.
// Compatable with (at least) the BMP085 and BMP180.
class Bmp180 {
 public:
  Bmp180(void);

  // Temperature.
  // // UNIT: degrees Celcius
  float temperature(void);

  // Pressure at the sensor.
  // UNIT: pascal (N/m^2)
  int32_t ambient_pressure(void);
  
  // Pressure altitude: altitude with altimeter setting at 101325 Pascals == 1013.25 millibars
  // == 29.92 inches mercury (i.e., std. pressure) // For pressure conversions, visit NOAA 
  // at: https://www.weather.gov/media/epz/wxcalc/pressureConversion.pdf.
  // (Pressure altitude is NOT correct for non-standard pressure or temperature.)
  // UNIT: meters
  float pressure_altitude(void);

  // Only if a humidity sensor is viable, TODO (Nolan Holden):
  // Add density altitude:
  // pressure altitude corrected for nonstandard temperature.
  // Remember: higher density altitude (High, Hot, and Humid) means decreased performance.

  // Disallow copying and moving.
  Bmp180(const Bmp180&) = delete;
  Bmp180& operator=(const Bmp180&) = delete;

 private:
  Adafruit_BMP085 bmp_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_BMP180_H_
