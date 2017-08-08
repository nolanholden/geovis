#include "atmospheric-sensor.h"

#include "constants.h"

namespace rcr {
namespace geovis {

namespace {
  constexpr const char* const kAtmDisplayName = "Atmospheric Sensor";
  constexpr const char* const KAtmCsvHeader = "Ambient Temperature (*Celcius),Ambient Pressure (Pascal) [raw],Ambient Pressure (Pascal) [filtered],Relative Humidity (%),Altitude (meters) [raw],Altitude (meters) [filtered],";

  constexpr uint8_t BME_SCK = 13;
  constexpr uint8_t BME_MISO = 1;
  constexpr uint8_t BME_MOSI = 0;
  constexpr uint8_t BME_CS = 31;

  constexpr float std_pressure = 1013.25f; // hecto-Pascals
} // namespace

AtmosphericSensor::AtmosphericSensor()
  : Sensor(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE, KALMAN_ERROR,
    kAtmDisplayName, KAtmCsvHeader) {
  bme_ = Adafruit_BME280(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
  altitude_ = kalmanInit(1.);
  pressure_ = kalmanInit(1.);
}

bool AtmosphericSensor::ProtectedInit() {
  return bme_.begin();
}

double AtmosphericSensor::ambient_pressure() {
  kalmanUpdate(&pressure_, (double)ambient_pressure_raw());
  return pressure_.value;
}

float AtmosphericSensor::ambient_pressure_raw() {
  return bme_.readPressure();
}

float AtmosphericSensor::humidity() {
  return bme_.readHumidity();
}

double AtmosphericSensor::pressure_altitude() {
  // @ std. pressure (i.e., 101325 Pa)
  kalmanUpdate(&altitude_, (double)pressure_altitude_raw());
  return altitude_.value; 
}

float AtmosphericSensor::pressure_altitude_raw() {
  return bme_.readAltitude(std_pressure); // std. pressure
}

float AtmosphericSensor::temperature() {
  return bme_.readTemperature();
}

String AtmosphericSensor::GetCsvLine() {
  String line = "";

  // Temperature (*C)
  line += temperature();
  line += ",";

  // Ambient pressure (Pascals)
  line += ambient_pressure_raw();
  line += ",";
  line += ambient_pressure();
  line += ",";

  // Relative humidity (%)
  line += humidity();
  line += ",";

  // Pressure altitude (meters)
  line += pressure_altitude_raw();
  line += ",";
  line += pressure_altitude();
  line += ",";

  return line;
}

} // namespace geovis
} // namespace rcr
