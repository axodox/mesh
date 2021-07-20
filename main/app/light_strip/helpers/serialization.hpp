#pragma once
#include "graphics/colors.hpp"
#include "numerics/float3.hpp"
#include "infrastructure/json.hpp"

namespace mesh::app::light_strip::helpers
{
  namespace color_rgb
  {
    std::unique_ptr<json::json_value> to_json(const graphics::color_rgb& value);
    graphics::color_rgb from_json(const json::json_value* json);
  }

  namespace float3
  {
    std::unique_ptr<json::json_value> to_json(const numerics::float3& value);
    numerics::float3 from_json(const json::json_value* json);
  }
}