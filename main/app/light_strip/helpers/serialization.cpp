#include "serialization.hpp"

using namespace mesh::graphics;
using namespace mesh::json;
using namespace std;

namespace mesh::app::light_strip::helpers
{
  std::unique_ptr<json::json_value> color_rgb::to_json(const graphics::color_rgb& color)
  {
    auto object = make_unique<json_object>();
    object->set_value("r", color.r);
    object->set_value("g", color.g);
    object->set_value("b", color.b);
    return object;
  }

  graphics::color_rgb color_rgb::from_json(const json::json_value* value)
  {
    graphics::color_rgb color{};
    if(value && value->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(value);
      object->get_value("r", color.r);
      object->get_value("g", color.g);
      object->get_value("b", color.b);
    }
    return color;
  }
}