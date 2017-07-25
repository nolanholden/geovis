#include "gps-receiver.h"

#include "constants.h"

namespace rcr {
namespace geovis {

namespace {
  constexpr const char* const kGpsDisplayName = "GPS Receiver";
  constexpr const char* const kGpsCsvHeader = "# Satellites Tracking,HDOP (Horizontal Dilution of Precision),Latitude,Longitude,Fix Age,Date (UTC),Time (UTC),Date Age,Altitude (meters) [raw],Altitude (meters),Vehicle Heading (degrees True),Vehicle Speed (knots) [raw],Vehicle Speed (knots),Characters Received,Sentences Received,# Checksum Failures,";

  constexpr uint8_t GPS_RX_PIN = 9; // Note: GPS module's TX connects to this pin.
  constexpr uint8_t GPS_TX_PIN = 10; // Note: GPS module's RX connects to this pin.
  constexpr uint32_t GPS_BAUD = 9600;
  // Note: All testing with Teensy 3.6 suggests
  // that baud rates other than 9600 are incompatible. However, 9600 is ok for
  // 5 Hz GPS refresh (which itself is very sufficient.)

  inline constexpr float gps_abs(float arg) { return arg < 0.f ? -arg : arg; }
  inline constexpr int gps_abs(int arg) { return arg < 0 ? -arg : arg; }
} // namespace

GpsReceiver::GpsReceiver()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR,
    kGpsDisplayName, kGpsCsvHeader) {
  latitude_ = 0.;
  longitude_ = 0.;

  speed_ = kalmanInit(0.);
  altitude_ = kalmanInit(0.);
}

bool GpsReceiver::Init() {
  Serial2.setRX(GPS_RX_PIN);
  Serial2.setTX(GPS_TX_PIN);
  Serial2.begin(GPS_BAUD);

  init_result_ = true;
  return init_result_;
}

double GpsReceiver::getLatitude() {
  if (gps_.location.isValid())
    latitude_ = gps_.location.lat();
  return latitude_;
}

double GpsReceiver::getLongitude() {
  if (gps_.location.isValid())
    longitude_ = gps_.location.lng();
  return longitude_;
}

double GpsReceiver::getSpeed() {
  if (gps_.speed.isValid()) {
    kalmanUpdate(&speed_, gps_.speed.knots());
  }
  return speed_.value;
}

double GpsReceiver::getAltitude() {
  if (gps_.altitude.isValid()) {
    kalmanUpdate(&altitude_, gps_.altitude.meters());
  }
  return altitude_.value;
}

void GpsReceiver::smartDelay() {
  while (Serial2.available()) gps_.encode(Serial2.read());
}

void GpsReceiver::smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (Serial2.available())
      gps_.encode(Serial2.read());
  } while (millis() - start < ms);
}

String GpsReceiver::getDateString(TinyGPSDate &d) {
  if (d.isValid()) {
    char sz[11];
    sprintf(sz, "%02d/%02d/%02d", d.month(), d.day(), d.year());
    return String{ sz };
  }
  return String{ "**/**/****" }; // same length
}
String GpsReceiver::getTimeString(TinyGPSTime &t) {
  if (t.isValid()) {
    char sz[9];
    sprintf(sz, "%02d:%02d:%02d", t.hour(), t.minute(), t.second());
    return String{ sz };
  }
  return String{ "**:**:**" }; // same length
}
String GpsReceiver::getDateAgeString(TinyGPSDate &d) {
  return getIntString(d.age(), d.isValid(), 5);
}
String GpsReceiver::getFloatString(float val, bool valid, int len, int prec) {
  String result = "";

  if (!valid) {
    while (len-- > 1)
      result += "*";
    result += " ";
  }
  else {
    result += String(val, prec);
    int vi = gps_abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // for "." and "-"
    flen += vi >= 1000 ? 4
      : vi >= 100 ? 3
      : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      result += " ";
  }

  return result;
}
String GpsReceiver::getIntString(unsigned long val, bool valid, int len) {
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';

  return String{ sz };
}
String GpsReceiver::getCstringString(const char *str, int len) {
  String result = "";
  int slen = strlen(str);
  for (auto i = 0; i<len; ++i)
    result += (i < slen ? str[i] : ' ');

  return result;
}

String GpsReceiver::GetCsvLine() {
  String line = "";

  line += getIntString(gps_.satellites.value(), gps_.satellites.isValid(), 5);
  line += ",";
  line += getIntString(gps_.hdop.value(), gps_.hdop.isValid(), 5);
  line += ",";
  line += getFloatString(gps_.location.lat(), gps_.location.isValid(), 11, 6);
  line += ",";
  line += getFloatString(gps_.location.lng(), gps_.location.isValid(), 12, 6);
  line += ",";
  line += getIntString(gps_.location.age(), gps_.location.isValid(), 5);
  line += ",";
  line += getDateString(gps_.date);
  line += ",";
  line += getTimeString(gps_.time);
  line += ",";
  line += getDateAgeString(gps_.date);
  line += ",";
  line += getFloatString(gps_.altitude.meters(), gps_.altitude.isValid(), 7, 2);
  line += ",";
  line += getAltitude(); // filtered
  line += ",";
  line += getFloatString(gps_.course.deg(), gps_.course.isValid(), 7, 2);
  line += ",";
  line += getFloatString(gps_.speed.knots(), gps_.speed.isValid(), 6, 2);
  line += ",";
  line += getSpeed(); // filtered
  line += ",";
  line += gps_.charsProcessed();
  line += ",";
  line += gps_.sentencesWithFix();
  line += ",";
  line += gps_.failedChecksum();
  line += ",";

  return line;
}

} // namespace geovis
} // namespace rcr
