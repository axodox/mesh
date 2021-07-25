#include "json_serialization.hpp"
#include "processors/json_serialization.hpp"
#include "sources/json_serialization.hpp"

using namespace std;
using namespace mesh::app::light_strip;
using namespace mesh::app::light_strip::processors;
using namespace mesh::app::light_strip::sources;

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<light_strip_settings>::to_json(const light_strip_settings& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("device", value.device);
    object->set_value("brightnessProcessor", value.brightness_processor);
    object->set_value("staticSource", value.static_source);
    object->set_value("rainbowSource", value.rainbow_source);
    return object;
  }

  bool json_serializer<light_strip_settings>::from_json(const std::unique_ptr<json_value>& json, light_strip_settings& value)
  {
    if (json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json.get());
      object->get_value("device", value.device);
      object->get_value("brightnessProcessor", value.brightness_processor);
      object->get_value("staticSource", value.static_source);
      object->get_value("rainbowSource", value.rainbow_source);
      return true;
    }
    else
    {
      return false;
    }
  }
}