#ifndef RCR_GEOVIS_SENSOR_H_
#define RCR_GEOVIS_SENSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "initializable.h"

namespace rcr {
namespace geovis {

class Sensor : public Initializable {
 public:
  Sensor(const char* display_name, const char* csv_header)
    : Initializable(display_name), kCsvHeader(csv_header) {}

  virtual String GetCsvLine() = 0;

  const char* const kCsvHeader;

  // Prevent base-call d'tor memory leak.
  virtual ~Sensor() {}
  
  // Disallow copying and moving.
  Sensor(const Sensor&) = delete;
  Sensor& operator=(const Sensor&) = delete;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_SENSOR_H_
