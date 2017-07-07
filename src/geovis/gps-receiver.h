#ifndef RCR_GEOVIS_GPSRECEIVER_H
#define RCR_GEOVIS_GPSRECEIVER_H

#include "sensor.h"
#include "tinygps-plus.h"

namespace rcr {
namespace geovis {

constexpr double PAD_LAT = 0.123456, PAD_LON = -0.123456;

constexpr uint8_t GPS_RX_PIN = 0; // Note: GPS module's TX connects to this pin.
constexpr uint8_t GPS_TX_PIN = 1; // Note: GPS module's RX connects to this pin.
constexpr int GPS_BAUD = 9600; // Note: All testing with Teensy 3.6 suggests...
// ...that baud rates other than 9600 are incompatible. However, 9600 is ok for
// 5 Hz GPS refresh (which itself is very sufficient.)

constexpr float OUT_OF_RANGE_DELTA = 0.001;
constexpr int OUT_OF_RANGE_LIMIT = 5;

class GpsReceiver : public virtual Sensor {
 public:
  GpsReceiver();
  bool Init();

  static constexpr const char* kCsvHeader = "satsTracking,hdop,lat,lon,fix-age,date,time,date-age,alt(m),gpsCourse,gpsSpeed,gpsCardinalHeading,distanceToPad,ourseToPad,cardinalToPad,CharsRX,SentencesRX,ChecksumFail"; // TODO: determine this.
  String GetCsvLine();

  float getAltitude();
  double getLatitude();
  double getLongitude();
  float getSpeed();

  // Use this instead of Arduino delay(x). Ensures that we continue listening
  // to the GPS module serial.
  void smartDelay();
  // Use this instead of Arduino delay(x). Ensures that we continue listening
  // to the GPS module serial.
  void smartDelay(unsigned long ms);

  static TinyGPSPlus gps_; // ok to access. User needs access to provide NMEA sentences

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


} // namespace level1_payload
} // namespace rcr

#endif // RCR_GEOVIS_GPSRECEIVER_H
