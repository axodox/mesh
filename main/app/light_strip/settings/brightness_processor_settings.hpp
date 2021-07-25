#pragma once
#include "numerics/float3.hpp"
#include "serialization/json.hpp"

#define ns mesh::app::light_strip::settings

namespace mesh::app::light_strip::settings
{
  struct brightness_processor_settings
  {
    numerics::float3 gamma{1.6f, 1.5f, 1.6f};
    float brightness = 1.f;
    float max_brightness = 0.7f;
  };
}

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::brightness_processor_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::brightness_processor_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::brightness_processor_settings& value);
  };
}

#undef ns