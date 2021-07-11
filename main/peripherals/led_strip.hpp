#pragma once
#include "driver/rmt.h"
#include "graphics/colors.hpp"
#include "infrastructure/array_view.hpp"

namespace mesh::peripherals
{
  class led_strip
  {
  public:
    virtual ~led_strip() = default;

    virtual void push_pixels(const infrastructure::array_view<graphics::color_rgb>& pixels) = 0;
  };
}