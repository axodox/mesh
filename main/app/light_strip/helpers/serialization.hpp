#pragma once
#include "graphics/colors.hpp"
#include "numerics/float3.hpp"
#include "infrastructure/json.hpp"

namespace mesh::app::light_strip::helpers
{
  std::unique_ptr<json::json_value> color_rgb_to_json(const graphics::color_rgb& value);
  void color_rgb_from_json(const json::json_value* json, graphics::color_rgb& value);

  std::unique_ptr<json::json_value> float3_to_json(const numerics::float3& value);
  void float3_from_json(const json::json_value* json, numerics::float3& value);
}