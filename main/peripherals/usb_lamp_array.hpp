#pragma once
#include <chrono>
#include <cstdint>

#define packed_struct struct __attribute__((packed))

namespace mesh::peripherals
{
  packed_struct lamp_array_bounding_box
  {
    uint32_t with, height, depth;
  };

  enum class lamp_array_kind : uint32_t
  {
    keyboard = 1,
    mouse = 2,
    game_controller = 3,
    peripheral = 4,
    scene = 5,
    notification = 6,
    chassis = 7,
    wearable = 8,
    furniture = 9,
    art = 10,
  };

  packed_struct lamp_position
  {
    uint32_t x, y, z;
  };

  enum class lamp_purposes : uint32_t
  {
    control = 1,
    accent = 2,
    branding = 4,
    status = 8,
    illumination = 16,
    presentation = 32,
  };

  packed_struct lamp_attributes
  {
    uint16_t id;
    lamp_position position;
    uint32_t update_latency;
    lamp_purposes purposes;
    uint8_t red_level_count;
    uint8_t green_level_count;
    uint8_t blue_level_count;
    uint8_t intensity_level_count;
    bool is_programmable;
    char lamp_key;
  };

  packed_struct lamp_color
  {
    union
    {
      struct
      {
        uint8_t r, g, b, w;
      };
      uint32_t v;
    };
  };

  uint32_t lamp_time(std::chrono::duration<uint32_t, std::micro> value)
  {
    return value.count();
  }

  const uint8_t lamp_array_attributes_report_id = 1;
  packed_struct lamp_array_attributes_report
  {
    const uint8_t report_id = lamp_array_attributes_report_id;
    uint16_t lamp_count;
    lamp_array_bounding_box size;
    lamp_array_kind kind;
    uint32_t min_update_interval;
  };

  const uint8_t lamp_attributes_request_report_id = 2;
  packed_struct lamp_attributes_request_report
  {
    const uint8_t report_id = lamp_attributes_request_report_id;
    uint16_t lamp_id;
  };

  const uint8_t lamp_attributes_response_report_id = 3;
  packed_struct lamp_attributes_response_report
  {
    const uint8_t report_id = lamp_attributes_response_report_id;
    lamp_attributes attributes;
  };

  const size_t lamp_multi_update_size = 8;

  enum class lamp_update_flags : uint8_t
  {
    none,
    complete
  };

  const uint8_t lamp_multi_update_report_id = 4;
  packed_struct lamp_multi_update_report
  {
    const uint8_t report_id = lamp_multi_update_report_id;
    uint8_t count;
    lamp_update_flags update_flags;
    uint16_t ids[lamp_multi_update_size];
    lamp_color colors[lamp_multi_update_size];
  };

  const uint8_t lamp_range_update_report_id = 5;
  packed_struct lamp_range_update_report
  {
    const uint8_t report_id = lamp_range_update_report_id;
    lamp_update_flags update_flags;
    uint16_t start;
    uint16_t end;
    lamp_color color;
  };

  const uint8_t lamp_array_control_report_id = 6;
  packed_struct lamp_array_control_report
  {
    const uint8_t report_id = lamp_array_control_report_id;
    bool autonomous_mode;
  };

  const uint16_t lamp_array_out_size = std::max({ sizeof(lamp_attributes_request_report), sizeof(lamp_array_control_report), sizeof(lamp_multi_update_report), sizeof(lamp_range_update_report) });
  const uint16_t lamp_array_in_size = std::max({ sizeof(lamp_array_attributes_report), sizeof(lamp_array_control_report), sizeof(lamp_attributes_response_report) });
}