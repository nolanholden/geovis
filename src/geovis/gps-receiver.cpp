#include "gps-receiver.h"

#include "constants.h"
#include "kalman.h"
#include "sensor.h"

#include <cstdio>
#include <utility>

namespace rcr {
namespace geovis {

constexpr double kGpsKalmanProcessNoise = 0.01;
constexpr double kGpsKalmanMeasurementNoise = 0.25;
constexpr double kGpsKalmanError = 1.;
static_assert(kGpsKalmanProcessNoise
  + kGpsKalmanMeasurementNoise
  + kGpsKalmanError != double{ 0 },
  "The sum of 'process noise covariance', 'measurement noise covariance', and 'estimation error covariance' cannot be zero; this creates a divide-by-zero condition.");

namespace {
  constexpr const char* const kGpsDisplayName = "GPS Receiver";
  constexpr const char* const kGpsCsvHeader = "altitude-isValid,altitude-isUpdated,altitude-age,altitude-meters,course-isValid,course-isUpdated,course-age,course-deg,date-isValid,date-isUpdated,date-age,time-isValid,time-isUpdated,time-age,iso8601,hdop-isValid,hdop-isUpdated,hdop-age,hdop-value,location-isValid,location-isUpdated,location-age,location-lat,location-lng,location-rawLat-billionths,location-rawLng-billionths,satellites_tracking-isValid,satellites_tracking-isUpdated,satellites_tracking-age,satellites_tracking-value,speed-isValid,speed-isUpdated,speed-age,speed-knots,GpsCharsProcessed,GpsSentencesWithFix,GpsFailedChecksums,GpsPassedChecksums,";
} // namespace

GpsReceiver::GpsReceiver(HardwareSerial& serial)
  : Sensor(kGpsDisplayName, kGpsCsvHeader), gps_serial_(serial),
    datetime_{ gps_.date, gps_.time } {}

void GpsReceiver::Update() {
  // Update Kalman filters *first*, s.t. newly encoded sentences remain "updated"
  // when GetCsvLine() is called.
  if (altitude().isValid()) {
    altitude_filtered_.Update(altitude().meters());
  }
  if (course().isValid()) {
    course_filtered_.Update(course().deg());
  }
  if (location().isValid()) {
    lat_filtered_.Update(location().lat());
    lng_filtered_.Update(location().lng());
  }
  if (speed().isValid()) {
    speed_filtered_.Update(speed().knots());
  }

  // Now encode available NMEA sentences.
  while (gps_serial_.available()) {
    gps_.encode(gps_serial_.read());
  }

  // Finally, update DateTime struct.
  datetime_.date = gps_.date;
  datetime_.time = gps_.time;
}

String GpsReceiver::GetCsvLine() {
  static String line = "";

  line += altitude().isValid();
  line += ",";
  line += altitude().isUpdated();
  line += ",";
  line += altitude().age();
  line += ",";
  line += altitude().meters();
  line += ",";

  line += course().isValid();
  line += ",";
  line += course().isUpdated();
  line += ",";
  line += course().age();
  line += ",";
  line += course().deg();
  line += ",";

  line += datetime().date.isValid();
  line += ",";
  line += datetime().date.isUpdated();
  line += ",";
  line += datetime().date.age();
  line += ",";
  line += datetime().time.isValid();
  line += ",";
  line += datetime().time.isUpdated();
  line += ",";
  line += datetime().time.age();
  line += ",";
  line += std::move(datetime().ToIso8601());
  line += ",";

  line += hdop().isValid();
  line += ",";
  line += hdop().isUpdated();
  line += ",";
  line += hdop().age();
  line += ",";
  line += hdop().value();
  line += ",";

  line += location().isValid();
  line += ",";
  line += location().isUpdated();
  line += ",";
  line += location().age();
  line += ",";
  line += location().lat();
  line += ",";
  line += location().lng();
  line += ",";
  line += location().rawLat().negative ? "-" : "+";
  line += ",";
  line += location().rawLat().billionths;
  line += ",";
  line += location().rawLng().negative ? "-" : "+";
  line += ",";
  line += location().rawLng().billionths;
  line += ",";

  line += satellites_tracking().isValid();
  line += ",";
  line += satellites_tracking().isUpdated();
  line += ",";
  line += satellites_tracking().age();
  line += ",";
  line += satellites_tracking().value();
  line += ",";

  line += speed().isValid();
  line += ",";
  line += speed().isUpdated();
  line += ",";
  line += speed().age();
  line += ",";
  line += speed().knots();
  line += ",";

  line += gps_.charsProcessed();
  line += ",";
  line += gps_.sentencesWithFix();
  line += ",";
  line += gps_.failedChecksum();
  line += ",";
  line += gps_.passedChecksum();
  line += ",";

  return line;
}

bool GpsReceiver::ProtectedInit() {
  gps_serial_.setRX(GPS_RX_PIN);
  gps_serial_.setTX(GPS_TX_PIN);
  gps_serial_.begin(GPS_BAUD);

  // Send commands to the Gps receiver.
  //  - Get RMC (recommended minimum) and GGA (fix data) data
  gps_serial_.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  //  - Refresh data 5 times per second
  gps_serial_.println(PMTK_SET_NMEA_UPDATE_5HZ);
  gps_serial_.println(PMTK_API_SET_FIX_CTL_5HZ);

  return true;
}


String DateTime::ToIso8601() const {
  static char iso_8601[kMaxIso8601Length];
  std::sprintf(iso_8601, "%04u-%02u-%02uT%02u:%02u:%02u.%02uZ",
    date.year(), date.month(), date.day(),
    time.hour(), time.minute(), time.second(), time.centisecond());
  return String{ iso_8601 };
}

} // namespace geovis
} // namespace rcr
