#include "tinyusb.h"
#include "usb_hid.hpp"

namespace mesh::peripherals
{
  uint8_t usb_service::add_string(const char* text)
  {
    auto result = uint8_t(_strings.size());
    _strings.push_back(text);
    return result;
  }
  
  usb_service::usb_service()
  {
    tinyusb_config_t usb_config = {
      .device_descriptor = nullptr,
      .string_descriptor = _strings.data(),
      .string_descriptor_count = int(_strings.size()),
    };
  }
}