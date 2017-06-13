#ifndef RCR_LEVEL1PAYLOAD_SDCARD_H_
#define RCR_LEVEL1PAYLOAD_SDCARD_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// SD card libraries
#include <BlockDriver.h>
//#include <FreeStack.h> // something is wrong with this library.
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SysCall.h>

#include "Setupable.h"

namespace rcr {
namespace level1payload {

// Encapsulates a BMP180 sensor and providing selected access to its interface.
// Compatable with (at least) the BMP085 and BMP180.
class SdCard : public Setupable {
 public:
  SdCard(void) {}

  // begin() the SdCard.
  void Setup(void);

  // Disallow copying and moving.
  SdCard(const Bmp180&) = delete;
  SdCard& operator=(const Bmp180&) = delete;

 private:
  SdFatSdio sd_card_;
};

} // namespace level1_payload
} // namespace rcr

#endif // RCR_LEVEL1PAYLOAD_SDCARD_H_
