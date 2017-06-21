#ifndef RCR_LEVEL1PAYLOAD_NOVELTYPRINTOUTS_H_
#define RCR_LEVEL1PAYLOAD_NOVELTYPRINTOUTS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace level1payload {

// Print message with a delayed, trailing ellipses.
template <typename T>
inline void print_with_ellipses(T message) {
  Serial.print(message);
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.println(".");
  delay(512);
}

// PROVISIONAL. REMOVE BEFORE FLIGHT. =^.^=
//void printBmeData(Adafruit_BME280& bme) {
//  Serial.print("Temperature = ");
//  Serial.print(bme.readTemperature());
//  Serial.println(" *C");
//
//  Serial.print("Ambient pressure = ");
//  Serial.print(bme.readPressure());
//  Serial.println(" Pa");
//
//  Serial.print("Relative humidity = ");
//  Serial.print(bme.readHumidity());
//  Serial.println(" percent");
//
//  Serial.print("Pressure altitude = ");
//  Serial.print(bme.readAltitude(1013.25f)); // 101325 Pa (i.e., std pressure)
//  Serial.println(" meters");
//  Serial.println();
//}

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_NOVELTYPRINTOUTS_H_
