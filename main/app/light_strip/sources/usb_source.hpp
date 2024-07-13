#pragma once
#include "light_source.hpp"
#include "infrastructure/logger.hpp"

namespace mesh::app::light_strip::sources::usb_lamp_array
{
  void enable();
  bool is_in_autonomous_mode();
  size_t lamp_count();
  std::span<const graphics::color_rgb> colors();
  std::span<const std::uint8_t> gains();
}