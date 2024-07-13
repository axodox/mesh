#include "usb_source_settings.hpp"
#include "app/light_strip/helpers/json_serialization.hpp"

using namespace std;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::settings
{
  light_source_type usb_source_settings::type() const
  {
    return light_source_type::usb_source;
  }
}

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<usb_source_settings>::to_json(const usb_source_settings& value)
  {
    return make_unique<json_object>();
  }

  bool json_serializer<usb_source_settings>::from_json(const std::unique_ptr<json_value>& json, usb_source_settings& value)
  {
    return json && json->type() == json_type::object;
  }
}