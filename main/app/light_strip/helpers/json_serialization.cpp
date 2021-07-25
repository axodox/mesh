#include "json_serialization.hpp"

using namespace mesh::graphics;
using namespace mesh::serialization::json;
using namespace std;

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<graphics::color_rgb>::to_json(const graphics::color_rgb& value)
  {
    auto json = make_unique<json_object>();
    json->set_value("r", value.r);
    json->set_value("g", value.g);
    json->set_value("b", value.b);
    return json;
  }

  bool json_serializer<graphics::color_rgb>::from_json(const std::unique_ptr<json_value>& json, graphics::color_rgb& value)
  {
    if(json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json.get());
      object->get_value("r", value.r);
      object->get_value("g", value.g);
      object->get_value("b", value.b);
      return true;
    }
    else
    {
      return false;
    }
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