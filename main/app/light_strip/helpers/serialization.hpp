#pragma once
#include "graphics/colors.hpp"
#include "infrastructure/json.hpp"

namespace mesh::app::light_strip::helpers
{
  namespace color_rgb
  {
    std::unique_ptr<json::json_value> to_json(const graphics::color_rgb& color);
    graphics::color_rgb from_json(const json::json_value* value);
  }
}