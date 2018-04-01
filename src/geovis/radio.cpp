#include "radio.h"
#include <type_traits>
#include "constants.h"

namespace rcr {
namespace stratocomm {

namespace {
  Radio* radio_instance;

  void on_receive(XBeeResponse& incoming_transmission, uintptr_t) {
    radio_instance->OnReceive(incoming_transmission);
  }
  constexpr uint32_t kXbeeBroadcastAddressMsb = 0x00000000;
  constexpr uint32_t kXbeeBroadcastAddressLsb = 0x0000ffff;
  constexpr uint32_t kXbeeSerialInterfaceBaudRate = 9600;
} // namespace

Radio::Radio(HardwareSerial & serial)
  : radio_serial_(serial) {
  addr64_.setMsb(kXbeeBroadcastAddressMsb);
  addr64_.setLsb(kXbeeBroadcastAddressLsb);
}

bool Radio::Init() {
  radio_serial_.begin(kXbeeSerialInterfaceBaudRate);
  xbee_.setSerial(radio_serial_);
  outbound_packet_.setAddress64(addr64_);
  radio_instance = this;
  xbee_.onOtherResponse(on_receive);

  return true;
}

void Radio::send_octet_buffer(uint8_t length) {
  outbound_packet_.setPayload(octets_buffer_, length);
  xbee_.send(outbound_packet_);
}

void Radio::Send(const String& text) {
  static_assert(kMaxPayloadLength > 0, "max payload cannot be zero, would cause zero division");
  using octet_t = std::decay<decltype(*octets_buffer_)>::type;

  const auto untrimmed_length = text.length(); // without terminator
  unsigned position = 0;
  const auto remainder = untrimmed_length % kMaxPayloadLength;
  const auto num_partitions = remainder == 0
    ? (untrimmed_length / kMaxPayloadLength) : (untrimmed_length / kMaxPayloadLength) + 1;
  for (auto partition = decltype(num_partitions){};
    partition < num_partitions; ++partition) {
    const auto length = partition + 1 < num_partitions
      ? kMaxPayloadLength : remainder;
    for (auto i = decltype(length){}; i < length; ++i) {
      octets_buffer_[i] = static_cast<octet_t>(text.charAt(position++));
    }
    send_octet_buffer(length);
  }
}

inline void Radio::OnReceive(XBeeResponse& incoming_transmission) {
  ZBRxResponse r;
  incoming_transmission.getZBRxResponse(r);

  auto get_string = [](const uint8_t* const octets, uint8_t length,
    uint8_t forward_offset = 0,
    uint8_t offset_from_back = 0) -> String {
    // [kMaxPayloadLength + 1] to include null terminator '\0'
    char buffer[kMaxPayloadLength + 1];

    const uint8_t* const trimmed_octets = &(octets[forward_offset]);

    // if new length is < 1 return empty string.
    if (length <= forward_offset + offset_from_back) {
      auto empty_string = String("");
      return empty_string;
    }

    const uint8_t trimmed_length = length - forward_offset - offset_from_back;
    for (uint8_t i = 0; i < trimmed_length; ++i) {
      buffer[i] = static_cast<char>(trimmed_octets[i]);
    }
    buffer[trimmed_length] = '\0'; // ensure strlen() will return correct length.

                                    // Convert to string.
    return String(buffer);
  };

  auto transmission = get_string(r.getData(), r.getDataLength());
  strcpy(char_buffer_, transmission.c_str());
  has_new_message_ = true;
}

const char* Radio::last_message() {
  has_new_message_ = false;
  return char_buffer_;
}

bool Radio::HasUnreadMessage() {
  return has_new_message_;
}

void Radio::Update() {
  xbee_.loop();
}

} // namespace stratocomm
} // namespace rcr
