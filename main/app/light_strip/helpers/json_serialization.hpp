#pragma once
#include "graphics/colors.hpp"
#include "numerics/float3.hpp"
#include "serialization/json.hpp"

namespace mesh::serialization::json
{
  template <>
  struct json_serializer<graphics::color_rgb>
  {
    static std::unique_ptr<json_value> to_json(const graphics::color_rgb& value);
    static bool from_json(const std::unique_ptr<json_value>& json, graphics::color_rgb& value);
  };

  template <>
  struct json_serializer<numerics::float3>
  {
    static std::unique_ptr<json_value> to_json(const numerics::float3& value);
    static bool from_json(const std::unique_ptr<json_value>& json, numerics::float3& value);
  };
}