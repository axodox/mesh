#include "json_serialization.hpp"
#include "app/light_strip/helpers/json_serialization.hpp"

using namespace std;
using namespace mesh::app::light_strip::processors;

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<brightness_processor_settings>::to_json(const brightness_processor_settings& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("gamma", value.gamma);
    object->set_value("brightness", value.brightness);
    object->set_value("maxBrightness", value.max_brightness);
    return object;
  }

  bool json_serializer<brightness_processor_settings>::from_json(const std::unique_ptr<json_value>& json, brightness_processor_settings& value)
  {
    if (json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json.get());
      object->get_value("gamma", value.gamma);
      object->get_value("brightness", value.brightness);
      object->get_value("maxBrightness", value.max_brightness);
      return true;
    }
    else
    {
      return false;
    }
  }
}