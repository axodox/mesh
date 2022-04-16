#pragma once
#include "serialization/json.hpp"

#define ns mesh::app::light_strip::settings

namespace mesh::app::light_strip::settings
{
  enum class light_source_type
  {
    empty_source,
    static_source,
    rainbow_source,
    udp_source,
    uart_source
  };

  struct light_source_settings
  {
    virtual ~light_source_settings() = default;
    virtual light_source_type type() const = 0;
  };

  struct empty_source_settings;
  struct static_source_settings;
  struct rainbow_source_settings;
  struct udp_source_settings;
  struct uart_source_settings;
}

namespace mesh::serialization::json
{
  template <>
  struct json_serializer<std::unique_ptr<ns::light_source_settings>> : public json_multi_serializer<
    ns::light_source_settings,
    ns::empty_source_settings,
    ns::static_source_settings,
    ns::rainbow_source_settings,
    ns::udp_source_settings,
    ns::uart_source_settings>
  { };
}

#undef ns