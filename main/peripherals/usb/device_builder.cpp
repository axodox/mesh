#include "device_builder.hpp"

namespace mesh::peripherals::usb
{
  device_builder::device_builder()
  {
    _strings.push_back("");
  }

  void device_builder::vendor(uint16_t id, const char* value)
  {
    _descriptor.vendor_id = id;
    set_string(_descriptor.vendor_name, value);
  }

  void device_builder::product(uint16_t id, const char* value)
  {
    _descriptor.product_id = id;
    set_string(_descriptor.product_name, value);
  }

  void device_builder::serial(const char* value)
  {
    set_string(_descriptor.serial_number, value);
  }

  configuration_builder device_builder::add_configuration()
  {
    return configuration_builder(*this);
  }

  void device_builder::set_string(uint8_t& id, const char* value)
  {
    if (value)
    {
      if (id == 0)
      {
        id = uint8_t(_strings.size());
        _strings.push_back(value);
      }
      else
      {
        _strings[id] = value;
      }
    }
    else
    {
      id = 0;
    }
  }
}