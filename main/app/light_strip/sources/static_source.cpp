#include "static_source.hpp"
#include "app\light_strip\helpers\serialization.hpp"

using namespace std;
using namespace mesh::json;
using namespace mesh::app::light_strip::helpers;

namespace mesh::app::light_strip::sources
{
  light_source_type static_source_settings::source_type() const
  {
    return light_source_type::static_source;
  }

  std::string static_source_settings::type_name() const
  {
    return "static";
  }

  std::unique_ptr<json_value> static_source_settings::to_json() const
  {
    auto object = make_unique<json_object>();
    add_type_info(object.get());
    object->set_value("color", color_rgb::to_json(color));
    return object;
  }

  void static_source_settings::from_json(const json_value* value)
  {
    if (value->type() != json_type::object) return;

    auto object = static_cast<const json_object*>(value);
    color_rgb::from_json(object->at("color").get(), color);
  }

  static_source::static_source()
  {
    static_source_settings default_settings{};
    apply_settings(&default_settings);
  }

  light_source_type static_source::source_type() const
  {
    return light_source_type::static_source;
  }

  void static_source::apply_settings(const light_source_settings* settings)
  {
    auto static_settings = static_cast<const static_source_settings*>(settings);
    color = static_settings->color;
  }

  void static_source::fill(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    for(auto& pixel : pixels)
    {
      pixel = color;
    }
  }
}