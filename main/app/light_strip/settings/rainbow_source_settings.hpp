#pragma once
#include <cmath>

#include "light_source_settings.hpp"
#include "app/light_strip/helpers/json_serialization.hpp"

#define ns mesh::app::light_strip::settings

namespace mesh::app::light_strip::settings
{
  struct rainbow_source_settings : public light_source_settings
  {
    inline static const char* type_name = "rainbow";

    uint8_t spatial_frequency = 1;
    float angular_velocity = float(M_PI);

    virtual light_source_type type() const override;
  };
}

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::rainbow_source_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::rainbow_source_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::rainbow_source_settings& value);
  };
}

#undef ns