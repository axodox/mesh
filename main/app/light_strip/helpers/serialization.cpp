#include "serialization.hpp"

using namespace mesh::graphics;
using namespace mesh::json;
using namespace std;

namespace mesh::app::light_strip::helpers
{
  std::unique_ptr<json::json_value> color_rgb::to_json(const graphics::color_rgb& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("r", value.r);
    object->set_value("g", value.g);
    object->set_value("b", value.b);
    return object;
  }

  void color_rgb::from_json(const json::json_value* json, graphics::color_rgb& value)
  {
    if(json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json);
      object->get_value("r", value.r);
      object->get_value("g", value.g);
      object->get_value("b", value.b);
    }
  }

  std::unique_ptr<json::json_value> float3::to_json(const numerics::float3& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("x", value.x);
    object->set_value("y", value.y);
    object->set_value("z", value.z);
    return object;
  }

  void float3::from_json(const json::json_value* json, numerics::float3& value)
  {
    if(json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json);
      object->get_value("x", value.x);
      object->get_value("y", value.y);
      object->get_value("z", value.z);
    }
  }
}