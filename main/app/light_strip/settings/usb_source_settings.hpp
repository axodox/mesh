#pragma once
#include "light_source_settings.hpp"

#define ns mesh::app::light_strip::settings

namespace mesh::app::light_strip::settings
{
  struct usb_source_settings : public light_source_settings
  {
    inline static const char* type_name = "usb";

    virtual light_source_type type() const override;
  };
}

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::usb_source_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::usb_source_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::usb_source_settings& value);
  };
}

#undef ns