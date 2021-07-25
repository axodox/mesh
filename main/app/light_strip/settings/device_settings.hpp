#pragma once
#include "serialization/json.hpp"

#define ns mesh::app::light_strip::settings

namespace mesh::app::light_strip::settings
{
  struct device_settings
  {
    uint32_t light_count = 4;
    std::chrono::milliseconds interval = std::chrono::milliseconds(16);
  };
}

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::device_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::device_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::device_settings& value);
  };
}

#undef ns