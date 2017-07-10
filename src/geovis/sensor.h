#ifndef RCR_GEOVIS_SENSOR_H_
#define RCR_GEOVIS_SENSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "initializable.h"
#include "kalman.h"

namespace rcr {
namespace geovis {

class Sensor : public virtual Initializable {
 public:
  Sensor(double process_noise, double measurement_noise, double error,
    const char* display_name);

  virtual String GetCsvLine() = 0;

  // Prevent base-call d'tor memory leak.
  virtual ~Sensor() noexcept {}
  
  // Disallow copying and moving.
  Sensor(const Sensor&) = delete;
  Sensor& operator=(const Sensor&) = delete;

 protected:
  kalman_t kalmanInit(double intial_value);
  void kalmanUpdate(kalman_t* state, double measurement);

 private:
  double processNoise_;
  double measurementNoise_;
  double error_;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_SENSOR_H_
