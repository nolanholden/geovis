#include "gps.h"

#include "constants.h"

inline constexpr float my_abs(float arg) { return arg < 0 ? -arg : arg; }
inline constexpr int my_abs(int arg) { return arg < 0 ? -arg : arg; }

namespace rcr {
namespace level1payload {

GpsReceiver::GpsReceiver() : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR) {
  latitude_ = 0;
  longitude_ = 0;

  speed_ = kalmanInit(0);
  altitude_ = kalmanInit(0);
}

bool GpsReceiver::Init() {
  Serial1.setRX(GPS_RX_PIN);
  Serial1.setTX(GPS_TX_PIN);
  Serial1.begin(GPS_BAUD);

  smartDelay(1000);
  return true;
}

double GpsReceiver::getLatitude() {
  smartDelay();
  if (gps_.location.isValid())
    latitude_ = gps_.location.lat();
  return latitude_;
}

double GpsReceiver::getLongitude() {
  if (gps_.location.isValid())
    longitude_ = gps_.location.lng();
  return longitude_;
}

float GpsReceiver::getSpeed() {
  if (gps_.speed.isValid()) {
    kalmanUpdate(&speed_, gps_.speed.knots());
  }
  return speed_.value;
}

inline void GpsReceiver::smartDelay() {
  while (Serial1.available())
    gps_.encode(Serial1.read());
}

inline void GpsReceiver::smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (Serial1.available())
      gps_.encode(Serial1.read());
  } while (millis() - start < ms);
}

float GpsReceiver::getAltitude() {
  if (gps_.altitude.isValid()) {
    kalmanUpdate(&altitude_, gps_.altitude.meters());
  }
  return altitude_.value;
}

String GpsReceiver::getDateString(TinyGPSDate &d) {
  smartDelay();

  if (d.isValid()) {
    char sz[11];
    sprintf(sz, "%02d/%02d/%02d", d.month(), d.day(), d.year());
    return String{ sz };
  }
  return String{ "**/**/****" }; // same length
}
String GpsReceiver::getTimeString(TinyGPSTime &t) {
  smartDelay();

  if (t.isValid()) {
    char sz[9];
    sprintf(sz, "%02d:%02d:%02d", t.hour(), t.minute(), t.second());
    return String{ sz };
  }
  return String{ "**:**:**" }; // same length
}
String GpsReceiver::getDateAgeString(TinyGPSDate &d) {
  smartDelay();
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
    int vi = my_abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // for "." and "-"
    flen += vi >= 1000 ? 4
      : vi >= 100 ? 3
      : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      result += " ";
  }

  smartDelay();
  return result;
}
String GpsReceiver::getIntString(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';

  smartDelay();
  return String{ sz };
}
String GpsReceiver::getCstringString(const char *str, int len)
{
  String result = "";
  int slen = strlen(str);
  for (auto i = 0; i<len; ++i)
    result += (i < slen ? str[i] : ' ');
  smartDelay();
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
line += getFloatString(gps_.course.deg(), gps_.course.isValid(), 7, 2);
line += ",";
line += getFloatString(gps_.speed.kmph(), gps_.speed.isValid(), 6, 2);
line += ",";
line += getCstringString(gps_.course.isValid() ? TinyGPSPlus::cardinal(gps_.course.deg()) : "*** ", 6);
line += ",";

unsigned long distance_to_pad =
(unsigned long)TinyGPSPlus::distanceBetween(
  gps_.location.lat(),
  gps_.location.lng(),
  PAD_LAT,
  PAD_LON) / 1000;

line += getIntString(distance_to_pad, gps_.location.isValid(), 9);
line += ",";

double course_to_pad = TinyGPSPlus::courseTo(
  gps_.location.lat(),
  gps_.location.lng(),
  PAD_LAT,
  PAD_LON);
line += getFloatString(course_to_pad, gps_.location.isValid(), 7, 2);
line += ",";

const char *cardinalToLondon = TinyGPSPlus::cardinal(course_to_pad);
line += getCstringString(gps_.location.isValid() ? cardinalToLondon : "*** ", 6);
line += ",";

line += getIntString(gps_.charsProcessed(), true, 6);
line += ",";
line += getIntString(gps_.sentencesWithFix(), true, 10);
line += ",";
line += getIntString(gps_.failedChecksum(), true, 9);
line += ",";

if (millis() > 5000 && gps_.charsProcessed() < 10)
  Serial.println(F("No GPS data received: check wiring"));

smartDelay();

return line;
}

} // namespace level1_payload
} // namespace rcr
