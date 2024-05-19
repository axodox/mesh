#pragma once
#include "driver/rmt.h"
#include "graphics/colors.hpp"
#include "infrastructure/array_view.hpp"
#include <span>

namespace mesh::peripherals
{
  class led_strip
  {
  public:
    virtual ~led_strip() = default;

    virtual void push_pixels(std::span<const graphics::color_rgb> pixels) = 0;
  };
}