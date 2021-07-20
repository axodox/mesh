#pragma once
#include "graphics/colors.hpp"
#include "infrastructure/array_view.hpp"

namespace mesh::app::light_strip::processors
{
  class color_processor
  {
  public:
    virtual void process(infrastructure::array_view<graphics::color_rgb>& pixels) = 0;
    virtual ~color_processor() = default;
  };
}