#include "json_serialization.hpp"
#include "infrastructure/text.hpp"

using namespace mesh::graphics;
using namespace mesh::infrastructure;
using namespace mesh::serialization::json;
using namespace std;

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<graphics::color_rgb>::to_json(const graphics::color_rgb& value)
  {
    auto text = format_text("#%.6x", value.r << 16 | value.g << 8 | value.b);
    return make_unique<json_string>(text);
  }

  bool json_serializer<graphics::color_rgb>::from_json(const std::unique_ptr<json_value>& json, graphics::color_rgb& value)
  {
    if(json && json->type() == json_type::string)
    {
      auto text = static_cast<const json_string*>(json.get())->value.c_str();
      auto value_as_number = 0u;
      if(sscanf(text, "#%6x", &value_as_number) == 1)
      {
        value.r = value_as_number >> 16 & 0xff;
        value.g = value_as_number >> 8 & 0xff;
        value.b = value_as_number & 0xff;
        return true;
      }      
    }

    return false;
  }

  std::unique_ptr<json_value> json_serializer<numerics::float3>::to_json(const numerics::float3& value)
  {
    auto json = std::make_unique<json_object>();
    json->set_value("x", value.x);
    json->set_value("y", value.y);
    json->set_value("z", value.z);
    return json;
  }

  bool json_serializer<numerics::float3>::from_json(const std::unique_ptr<json_value>& json, numerics::float3& value)
  {
    if (json && json->type() == json_type::object)
    {
      auto object = static_cast<json_object*>(json.get());
      object->get_value("x", value.x);
      object->get_value("y", value.y);
      object->get_value("z", value.z);
      return true;
    }
    else
    {
      return false;
    }
  }
}