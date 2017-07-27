#ifndef RCR_GEOVIS_GPSRECEIVER_H
#define RCR_GEOVIS_GPSRECEIVER_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"
#include "tinygps-plus.h"
#include "updateable.h"

namespace rcr {
namespace geovis {

class GpsReceiver : public virtual Sensor, public virtual Updateable {
 public:
  GpsReceiver();
  bool Init();

  String GetCsvLine();

  double getAltitude();
  double getLatitude();
  double getLongitude();
  double getSpeed();

  void Update();

 protected:
  TinyGPSPlus gps_;

  double latitude_;
  double longitude_;
  kalman_t speed_;    // knots
  kalman_t altitude_; // meters

  String getDateString(TinyGPSDate &d);
  String getTimeString(TinyGPSTime &t);
  String getDateAgeString(TinyGPSDate &d);
  String getFloatString(float val, bool valid, int len, int prec);
  String getIntString(unsigned long val, bool valid, int len);
  String getCstringString(const char *str, int len);
};


} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GPSRECEIVER_H
