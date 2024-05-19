#pragma once
#include "numerics/float3.hpp"
#include "serialization/json.hpp"
#include "graphics/gamma_correction.hpp"

#define ns mesh::app::light_strip::settings

namespace mesh::app::light_strip::settings
{
  using brightness_processor_settings = graphics::gamma_correction_settings;
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