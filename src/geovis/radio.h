#ifndef RCR_STRATOCOMM_RADIO_H_
#define RCR_STRATOCOMM_RADIO_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "constants.h"
#include "initializable.h"
#include "updateable.h"

#include "xbee.h"

namespace rcr {
namespace stratocomm {

class Radio : public virtual Initializable, public virtual Updateable {
 public:
  Radio(HardwareSerial& serial);

  bool Init();

  void Send(const String& text);

  void OnReceive(XBeeResponse& incoming_transmission);

  const char* last_message();

  bool HasUnreadMessage();

  void Update();

 private:
  XBeeWithCallbacks xbee_;
  HardwareSerial& radio_serial_;
  bool has_new_message_ = false;

  // reused outbound packet
  ZBTxRequest outbound_packet_;

  // SH & SL Address of receiving XBee
  XBeeAddress64 addr64_; // "broadcast" address

  // reused buffer
  uint8_t octets_buffer_[kMaxPayloadLength];

  // reused buffer
  // [kMaxPayloadLength + 1] to include null terminator '\0'
  char char_buffer_[kMaxPayloadLength + 1] = "\0";
};

} // namespace rcr
} // namespace stratocomm

#endif // RCR_STRATOCOMM_RADIO_H_
