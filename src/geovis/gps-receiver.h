#ifndef RCR_GEOVIS_GPSRECEIVER_H
#define RCR_GEOVIS_GPSRECEIVER_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "constants.h"
#include "kalman.h"
#include "sensor.h"
#include "tinygps-plus.h"
#include "updateable.h"

namespace rcr {
namespace geovis {

constexpr size_t kMaxIso8601Length = 33;

extern const double
  kGpsKalmanProcessNoise,
  kGpsKalmanMeasurementNoise,
  kGpsKalmanError;

// Last-updated date & time.
struct DateTime {
  TinyGPSDate& date;
  TinyGPSTime& time;

  // e.g., "2017-08-05T23:16:52.11Z" (represents 2017 Aug 5, 11:16:52.11 PM Zulu)
  String ToIso8601() const;
};

// High-level wrapper for Gps receiver.
class GpsReceiver : public Sensor, public Updateable {
 public:
  GpsReceiver(HardwareSerial& serial);

  String GetCsvLine();

  void Update();

  const TinyGPSAltitude& altitude() const           { return gps_.altitude; }
  const TinyGPSCourse& course() const               { return gps_.course; }
  const DateTime& datetime() const                  { return datetime_; }
  const TinyGPSDecimal& hdop() const                { return gps_.hdop; }
  const TinyGPSLocation& location() const           { return gps_.location; }
  const TinyGPSInteger& satellites_tracking() const { return gps_.satellites; }
  const TinyGPSSpeed& speed() const                 { return gps_.speed; }

 private:
  HardwareSerial& gps_serial_;

  // The below library almost provides desired quailities, but we will use
  // it compositionally here to increase encapsulation and maintain
  // polymorphism.
  TinyGPSPlus gps_;

  DateTime datetime_;

  // Kalman filtered values.
  // Units: [m], [360deg], [deg North (+/-)], [deg East (+/-)], [knots]
  // i.e., South of equator and West of prime-meridian are negative degrees.
  Kalman<double, kGpsKalmanProcessNoise, kGpsKalmanMeasurementNoise, kGpsKalmanError>
    altitude_filtered_, course_filtered_, lat_filtered_, lng_filtered_, speed_filtered_;

  bool ProtectedInit();
};


} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_GPSRECEIVER_H
