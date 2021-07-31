#pragma once
#include "device_settings.hpp"
#include "brightness_processor_settings.hpp"
#include "empty_source_settings.hpp"
#include "static_source_settings.hpp"
#include "rainbow_source_settings.hpp"
#include "udp_source_settings.hpp"

#define ns mesh::app::light_strip::settings

namespace mesh::app::light_strip::settings
{
  struct light_strip_settings
  {
    device_settings device{};
    brightness_processor_settings brightness_processor{};

    light_source_type source_type = light_source_type::empty_source;
    empty_source_settings empty_source{};
    static_source_settings static_source{};
    rainbow_source_settings rainbow_source{};
    udp_source_settings udp_source{};
  };
}

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::light_strip_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::light_strip_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::light_strip_settings& value);
  };
}

#undef ns