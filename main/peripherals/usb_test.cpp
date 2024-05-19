#include "usb_test.hpp"
#include "usb_descriptors.hpp"
#include "usb_lamp_array.hpp"
#include "usb_lamp_array_descriptor.hpp"
#include <array>
#include <esp_err.h>
#include <span>
#include <tinyusb.h>

using namespace mesh::peripherals;
using namespace std;

const char* string_descriptor[6] = {
  "\x09\x04",                  // 0: is supported language is English (0x0409)
  "Axodox",                    // 1: Manufacturer
  "Lux Light Array",           // 2: Product
  "000001",                    // 3: Serials, should use chip ID
  "Light strip HID interface", // 4: HID
  "Default configuration"      // 5: Configuration
};

const usb_device_descriptor device_descriptor{
  .vendor_id = 0xa02f,
  .product_id = 0x0002,
  .device_id = 0x0001,

  .vendor_name = 1,
  .product_name = 2,
  .serial_number = 3,

  .configuration_count = 1,
};

const packed_struct lamp_array_configuration
{
  usb_configuration_descriptor configuration;
  usb_interface_descriptor interface;
  usb_hid_descriptor hid;
  usb_endpoint_descriptor endpoint_in;
  usb_endpoint_descriptor endpoint_out;
}
hid_configuration_descriptor{
  .configuration = {
    .total_length = sizeof(lamp_array_configuration),
    .interface_count = 1,
    .id = 1,
    .name = 5
  },
  .interface = {
    .endpoint_count = 2,
    .kind = usb_interface_type::hid,
    .name = 4
  }, 
  .hid = {
    .descriptor_count = 1,
    .descriptor_type = usb_descriptor_type::report,
    .descriptor_length = uint16_t(size(lamp_array_report_descriptor)),
  },  
  .endpoint_in = {
    .address = usb_endpoint_address(1, usb_endpoint_direction::in),
    .attributes = usb_endpoint_attributes::interrupt,
    .max_packet_size = 64,
  },
  .endpoint_out = {
    .address = usb_endpoint_address(2, usb_endpoint_direction::out),
    .attributes = usb_endpoint_attributes::interrupt,
    .max_packet_size = 64,
  },
};

const uint8_t* tud_hid_descriptor_report_cb(uint8_t instance)
{
  return lamp_array_report_descriptor;
}

template <typename T> T* allocate_report(span<uint8_t> buffer)
{
  //assert(buffer.size() >= sizeof(T));
  return new (buffer.data()) T();
}

uint16_t get_lamp_array_attributes_report(span<uint8_t> buffer)
{
  auto report = allocate_report<lamp_array_attributes_report>(buffer);
  report->lamp_count = 1;
  report->size = { 1, 1, 1 };
  report->kind = lamp_array_kind::scene;
  report->min_update_interval = lamp_time(1s);
  return uint16_t(sizeof(lamp_array_attributes_report));
}

uint16_t lamp_id = 0;
bool autonomous_mode = true;
lamp_color color;

uint16_t get_lamp_attributes_response_report(span<uint8_t> buffer)
{
  auto report = allocate_report<lamp_attributes_response_report>(buffer);
  report->attributes = {
    .id = lamp_id,
    .position = { 0, 0, 0 },
    .update_latency = lamp_time(2ms),
    .purposes = lamp_purposes::presentation,
    .red_level_count = 255,
    .green_level_count = 255,
    .blue_level_count = 255,
    .intensity_level_count = 1,
    .is_programmable = true,
    .lamp_key = 0,
  };
  return uint16_t(sizeof(lamp_attributes_response_report));
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t length)
{
  span<uint8_t> report_buffer{ buffer, length };

  switch (report_id)
  {
  case lamp_array_attributes_report_id:
    return get_lamp_array_attributes_report(report_buffer);
  case lamp_attributes_response_report_id:
    return get_lamp_attributes_response_report(report_buffer);
  }
  return 0;
}

template <typename T> const T* read_report(span<const uint8_t> buffer)
{
  //assert(buffer.size() >= sizeof(T));
  return reinterpret_cast<const T*>(buffer.data());
}

void set_lamp_attributes_request_report(span<const uint8_t> buffer)
{
  auto report = read_report<lamp_attributes_request_report>(buffer);
  lamp_id = report->lamp_id;
}

void set_lamp_array_control_report(span<const uint8_t> buffer)
{
  auto report = read_report<lamp_array_control_report>(buffer);
  autonomous_mode = report->autonomous_mode;
}

void set_lamp_multi_update_report(span<const uint8_t> buffer)
{
  auto report = read_report<lamp_multi_update_report>(buffer);
  if (report->count < 1) return;

  color = report->colors[0];
  printf("Color set %04lx\n", color.v);
}

void set_lamp_range_update_report(span<const uint8_t> buffer)
{
  auto report = read_report<lamp_range_update_report>(buffer);
  if (report->start != 0 && report->end >= 1) return;

  color = report->color;
  printf("Color set %04lx\n", color.v);
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, const uint8_t* buffer, uint16_t length)
{
  span<const uint8_t> report_buffer{ buffer, length };

  switch (report_id)
  {
  case lamp_attributes_request_report_id:
    set_lamp_attributes_request_report(report_buffer);
    break;
  case lamp_array_control_report_id:
    set_lamp_array_control_report(report_buffer);
    break;
  case lamp_multi_update_report_id:
    set_lamp_multi_update_report(report_buffer);
    break;
  case lamp_range_update_report_id:
    set_lamp_range_update_report(report_buffer);
    break;
  }
}

void setup_usb()
{
  tinyusb_config_t usb_config{
    .device_descriptor = device_descriptor,
    .string_descriptor = string_descriptor,
    .string_descriptor_count = size(string_descriptor),
    .external_phy = false,
    .configuration_descriptor = reinterpret_cast<const uint8_t*>(&hid_configuration_descriptor),
    .self_powered = false,
    .vbus_monitor_io = -1,
  };

  ESP_ERROR_CHECK(tinyusb_driver_install(&usb_config));
  printf("USB initialization DONE\n");
}