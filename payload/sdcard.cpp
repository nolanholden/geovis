#include "sdcard.h"

namespace rcr {
namespace level1payload {

void SdCard::Setup() {
  // Setup MicroSD card I/O. Trap the thread if no card is found.
  if (!sd_card.begin()) {
    Serial.println("ERROR: SD card could not be found.");
    while (1) { /* Trap the thread. */ }
  }
  Serial.println("SdCard successfully setup.");
  Serial.println();
}

} // namespace level1_payload
} // namespace rcr
