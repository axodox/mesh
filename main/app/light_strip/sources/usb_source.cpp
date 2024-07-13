#include "usb_source.hpp"
#include "infrastructure/bitwise_operations.hpp"
#include "peripherals/usb_descriptors.hpp"
#include "peripherals/usb_lamp_array.hpp"
#include "peripherals/usb_lamp_array_descriptor.hpp"
#include <array>
#include <span>
#include <tinyusb.h>

using namespace std;
using namespace mesh::app::light_strip::settings;
using namespace mesh::graphics;
using namespace mesh::infrastructure;
using namespace mesh::numerics;
using namespace mesh::peripherals;

namespace {
  //Constants & descriptors
  const uint16_t lamp_group_size = 3;
  const uint16_t lamp_count = 219;
  const uint16_t lamp_group_count = lamp_count / lamp_group_size;

  const lamp_array_bounding_box lamp_bounding_box = { 123_cm, 2_cm, 70_cm };
  const lamp_position lamp_start_position = { 2_cm, 1_cm, 68_cm };
  const light_segment lamp_segments[] = {
    { .light_count = 39, .end_position = { 2_cm, 1_cm, 2_cm } },
    { .light_count = 70, .end_position = { 121_cm, 1_cm, 2_cm } },
    { .light_count = 39, .end_position = { 121_cm, 1_cm, 68_cm } },
    { .light_count = 71, .end_position = { 2_cm, 1_cm, 68_cm } },
  };

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
    .product_id = 0x0007,
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
      .interval = 1,
    },
    .endpoint_out = {
      .address = usb_endpoint_address(2, usb_endpoint_direction::out),
      .attributes = usb_endpoint_attributes::interrupt,
      .max_packet_size = 64,
      .interval = 1,
    },
  };

  //Interrupt globals
  color_rgb source_colors[lamp_count];
  uint8_t gains[lamp_count];
  lamp_position lamp_positions[lamp_count];
  
  uint16_t lamp_id = 0;
  bool autonomous_mode = true;

  //Interrupt report builders
  uint16_t get_lamp_array_attributes_report(span<uint8_t> buffer)
  {
    auto report = allocate_value<lamp_array_attributes_report>(buffer);
    report->lamp_count = lamp_group_count;
    report->size = lamp_bounding_box;
    report->kind = lamp_array_kind::peripheral;
    report->min_update_interval = lamp_time(1ms);
    return uint16_t(sizeof(lamp_array_attributes_report));
  }

  uint16_t get_lamp_attributes_response_report(span<uint8_t> buffer)
  {
    auto report = allocate_value<lamp_attributes_response_report>(buffer);
    report->attributes = {
      .id = lamp_id,
      .position = lamp_positions[lamp_id * lamp_group_size],
      .update_latency = lamp_time(32ms),
      .purposes = lamp_purposes::accent,
      .red_level_count = 255,
      .green_level_count = 255,
      .blue_level_count = 255,
      .intensity_level_count = 1,
      .is_programmable = true,
      .lamp_key = 0,
    };

    lamp_id++;
    if (lamp_id == lamp_group_count) lamp_id = 0;

    return uint16_t(sizeof(lamp_attributes_response_report));
  }

  void set_lamp_attributes_request_report(span<const uint8_t> buffer)
  {
    auto report = read_value<lamp_attributes_request_report>(buffer);
    lamp_id = report->lamp_id;
  }

  void set_lamp_array_control_report(span<const uint8_t> buffer)
  {
    auto report = read_value<lamp_array_control_report>(buffer);
    autonomous_mode = report->autonomous_mode;

    if (autonomous_mode)
    {
      ranges::fill(source_colors, color_rgb{});
      ranges::fill(gains, 0);
    }
  }

  void IRAM_ATTR set_lamp_multi_update_report(span<const uint8_t> buffer)
  {
    auto report = read_value<lamp_multi_update_report>(buffer);

    if (report->count > 0)
    {
      auto count = min(report->count, lamp_multi_update_size);
      for (auto i = 0; i < count; i++)
      {
        auto lamp_index = report->ids[i];
        if (lamp_index >= lamp_group_count) continue;

        auto lamp_color = report->colors[i];
        for (auto j = 0; j < lamp_group_size; j++)
        {
          source_colors[lamp_index * lamp_group_size + j] = { lamp_color.r, lamp_color.g, lamp_color.b };
          gains[lamp_index * lamp_group_size + j] = lamp_color.w;
        }
      }
    }
  }

  void IRAM_ATTR set_lamp_range_update_report(span<const uint8_t> buffer)
  {
    auto report = read_value<lamp_range_update_report>(buffer);

    auto start = min(report->start, lamp_group_count);
    auto end = min(report->end, lamp_group_count);

    if (start < end)
    {
      for (auto i = start; i <= end; i++)
      {
        for (auto j = 0; j < lamp_group_size; j++)
        {
          source_colors[i * lamp_group_size + j] = { report->color.r, report->color.g, report->color.b };
          gains[i * lamp_group_size + j] = report->color.w;
        }
      }
    }
  }

  float3 to_position(lamp_position value)
  {
    return { float(value.x), float(value.y), float(value.z) };
  }

  lamp_position to_position(float3 value)
  {
    return { uint32_t(value.x), uint32_t(value.y), uint32_t(value.z) };
  }

  void set_lamp_positions(span<lamp_position> positions, lamp_position start, span<const light_segment> segments)
  {
    float3 position = to_position(start);

    auto it = positions.begin();
    for (auto segment : segments)
    {
      float3 step = (to_position(segment.end_position) - position) / (segment.light_count - 1);
      for (auto i = 0; i < segment.light_count; i++)
      {
        position += step;
        *it++ = to_position(position);
      }
    }
  }
}

//Interrupt handlers

const uint8_t* tud_hid_descriptor_report_cb(uint8_t instance)
{
  return lamp_array_report_descriptor;
}

void IRAM_ATTR tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, const uint8_t* buffer, uint16_t length)
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

namespace mesh::app::light_strip::sources
{
  void enable_usb_lamp_array()
  {
    set_lamp_positions(lamp_positions, lamp_start_position, lamp_segments);

    ranges::fill(source_colors, color_rgb{});
    ranges::fill(gains, 0);

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
  }

  bool is_in_usb_lamp_array_autonomous_mode()
  {
    return autonomous_mode;
  }

  usb_source::usb_source(light_strip_context& context) :
    light_source(context)
  { }

  light_source_type usb_source::type() const
  {
    return light_source_type::usb_source;
  }

  const light_source_settings* usb_source::get_settings() const
  {
    return &_context.settings.usb_source;
  }

  void usb_source::fill(std::span<graphics::color_rgb> pixels)
  {
    memcpy(pixels.data(), source_colors, min(size(source_colors), pixels.size()) * sizeof(color_rgb));
  }
}