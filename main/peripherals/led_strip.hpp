#pragma once
#include "driver/rmt.h"
#include "infrastructure/array_view.hpp"

namespace mesh::peripherals
{
  struct color_rgb
  {
    uint8_t g, r, b;

    color_rgb() = default;
    color_rgb(uint8_t r, uint8_t g, uint8_t b) :
      g(g),
      r(r),
      b(b)
    { }
  };

  class led_strip
  {
  public:
    virtual ~led_strip() = default;

    virtual void push_pixels(const infrastructure::array_view<color_rgb>& pixels) = 0;
  };
}