#pragma once
#include <cstdint>
#include <vector>

#define packed_struct struct __attribute__((packed))

namespace mesh::peripherals::usb
{
  using string_index = uint8_t;

  enum class descriptor_type : uint8_t
  {
    none = 0x00,
    configuration = 0x02,
    interface = 0x04,
    endpoint = 0x05,
    hid = 0x21,
    report = 0x22
  };

  //Configuration

  enum class configuration_attributes : uint8_t
  {
    none = 0,
    remote_wakeup = 32
  };

  packed_struct configuration_descriptor
  {
  private:
    uint8_t _length = 9;
    descriptor_type _type = descriptor_type::configuration;

  public:
    uint16_t total_length = 0;
    uint8_t interface_count = 0; 
    uint8_t id = 0;
    string_index name = 0;
    configuration_attributes attributes = configuration_attributes::remote_wakeup;
    uint8_t power = 100;
  };

  static_assert(sizeof(configuration_descriptor) == 9);

  //Interface

  enum class interface_type : uint8_t
  {
    none = 0,
    hid = 3
  };

  packed_struct interface_descriptor
  {
  private:
    uint8_t _length = 9;
    descriptor_type _type = descriptor_type::interface;

  public:
    uint8_t id = 0;
    uint8_t alternate_setting = 0;
    uint8_t endpoint_count = 0;
    interface_type type = interface_type(0);
    uint8_t subtype = 0;
    uint8_t protocol = 0;
    string_index name = 0;
  };

  static_assert(sizeof(interface_descriptor) == 9);

  //Endpoint

  enum class endpoint_direction : uint8_t
  {
    out = 0,
    in = 127
  };

  uint8_t endpoint_address(uint8_t lane, endpoint_direction direction);

  enum class endpoint_attributes : uint8_t
  {
    control = 0,
    isochronous,
    bulk,
    interrupt
  };

  packed_struct endpoint_descriptor
  {
  private:
    uint8_t _length = 7;
    descriptor_type _type = descriptor_type::endpoint;

  public:
    uint8_t address = 0;
    endpoint_attributes attributes = endpoint_attributes::interrupt;
    uint16_t max_packet_size = 0;
    uint8_t interval = 10;
  };

  static_assert(sizeof(endpoint_descriptor) == 7);

  //HID

  packed_struct hid_descriptor
  {
  private:
    uint8_t _length = 9;
    descriptor_type _type = descriptor_type::hid;

  public:
    uint16_t version = 0x0111;
    uint8_t country_code = 0;
    uint8_t descriptor_count = 0;
    descriptor_type descriptor_type = descriptor_type::none;
    uint16_t descriptor_length = 0;
  };

  static_assert(sizeof(hid_descriptor) == 9);
  
}
