#include "device_settings.hpp"

using namespace std;
using namespace mesh::app::light_strip::settings;

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<device_settings>::to_json(const device_settings& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("lightCount", value.light_count);
    object->set_value("interval", value.interval);
    return object;
  }

  bool json_serializer<device_settings>::from_json(const std::unique_ptr<json_value>& json, device_settings& value)
  {
    if (json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json.get());
      object->get_value("lightCount", value.light_count);
      object->get_value("interval", value.interval);
      return true;
    }
    else
    {
      return false;
    }
  }
}