#pragma once
#include "light_source_settings.hpp"
#include "graphics/colors.hpp"

#define ns mesh::app::light_strip::settings

namespace mesh::app::light_strip::settings
{
  struct uart_source_settings : public light_source_settings
  {
    inline static const char* type_name = "uart";

    uint64_t baud_rate = 115200;

    virtual light_source_type type() const override;
  };
}

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::uart_source_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::uart_source_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::uart_source_settings& value);
  };
}

#undef ns