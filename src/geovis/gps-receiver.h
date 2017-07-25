#ifndef RCR_GEOVIS_GPSRECEIVER_H
#define RCR_GEOVIS_GPSRECEIVER_H

#include "sensor.h"
#include "tinygps-plus.h"

namespace rcr {
namespace geovis {

class GpsReceiver : public virtual Sensor {
 public:
  GpsReceiver();
  bool Init();

  String GetCsvLine();

  double getAltitude();
  double getLatitude();
  double getLongitude();
  double getSpeed();

  // Use this instead of Arduino delay(x). Ensures that we continue listening
  // to the GPS module serial.
  void smartDelay();
  // Use this instead of Arduino delay(x). Ensures that we continue listening
  // to the GPS module serial.
  void smartDelay(unsigned long ms);

  TinyGPSPlus gps_; // ok to access. User needs access to provide NMEA sentences

 protected:
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
