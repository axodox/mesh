#pragma once
#include <cstdint>
#include <vector>
#include <tinyusb.h>

#define packed_struct struct __attribute__((packed))

namespace mesh::peripherals
{
  using string_index = uint8_t;

  enum class usb_descriptor_type : uint8_t
  {
    none = 0x00,
    device = 0x01,
    configuration = 0x02,
    interface = 0x04,
    endpoint = 0x05,
    hid = 0x21,
    report = 0x22
  };

  // Device

  packed_struct usb_device_descriptor
  {
    const uint8_t length = 18;
    const usb_descriptor_type type = usb_descriptor_type::device;

    uint16_t usb_specification = 0x0200;

    uint8_t device_class = 0;
    uint8_t device_sub_class = 0;
    uint8_t device_protocol = 0;

    uint8_t max_packet_size = 64;

    uint16_t vendor_id = 0;
    uint16_t product_id = 0;
    uint16_t device_id = 0;

    uint8_t vendor_name = 0;
    uint8_t product_name = 0;
    uint8_t serial_number = 0;

    uint8_t configuration_count = 0;

    operator const tusb_desc_device_t*() const
    {
      return reinterpret_cast<const tusb_desc_device_t*>(this);
    }
  };

  static_assert(sizeof(usb_device_descriptor) == 18);

  // Configuration

  enum class usb_configuration_attributes : uint8_t
  {
    none = 0,
    remote_wakeup = 32
  };

  packed_struct usb_configuration_descriptor
  {
    const uint8_t length = 9;
    const usb_descriptor_type type = usb_descriptor_type::configuration;

    uint16_t total_length = 0;
    uint8_t interface_count = 0;
    uint8_t id = 0;
    string_index name = 0;
    usb_configuration_attributes attributes = usb_configuration_attributes::none;
    uint8_t power = 50;
  };

  static_assert(sizeof(usb_configuration_descriptor) == 9);

  // Interface

  enum class usb_interface_type : uint8_t
  {
    none = 0,
    hid = 3
  };

  packed_struct usb_interface_descriptor
  {
    const uint8_t length = 9;
    const usb_descriptor_type type = usb_descriptor_type::interface;

    uint8_t id = 0;
    uint8_t alternate_setting = 0;
    uint8_t endpoint_count = 0;
    usb_interface_type kind = usb_interface_type::none;
    uint8_t subtype = 0;
    uint8_t protocol = 0;
    string_index name = 0;
  };

  static_assert(sizeof(usb_interface_descriptor) == 9);

  // Endpoint

  enum class usb_endpoint_direction : uint8_t
  {
    out = 0,
    in = 128
  };

  constexpr uint8_t usb_endpoint_address(uint8_t lane, usb_endpoint_direction direction)
  {
    return (lane & 0xFF) | uint8_t(direction);
  }

  enum class usb_endpoint_attributes : uint8_t
  {
    control = 0,
    isochronous,
    bulk,
    interrupt
  };

  packed_struct usb_endpoint_descriptor
  {
    const uint8_t length = 7;
    const usb_descriptor_type type = usb_descriptor_type::endpoint;

    uint8_t address = 0;
    usb_endpoint_attributes attributes = usb_endpoint_attributes::control;
    uint16_t max_packet_size = 0;
    uint8_t interval = 10;
  };

  static_assert(sizeof(usb_endpoint_descriptor) == 7);

  // HID

  packed_struct usb_hid_descriptor
  {
    const uint8_t length = 9;
    const usb_descriptor_type type = usb_descriptor_type::hid;

    uint16_t version = 0x0111;
    uint8_t country_code = 0;
    uint8_t descriptor_count = 0;
    usb_descriptor_type descriptor_type = usb_descriptor_type::none;
    uint16_t descriptor_length = 0;
  };

  static_assert(sizeof(usb_hid_descriptor) == 9);

  const uint8_t* as_bytes(const auto& value)
  {
    return reinterpret_cast<const uint8_t*>(&value);
  }
}
