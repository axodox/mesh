#include "rainbow_source.hpp"
#include "app\light_strip\helpers\serialization.hpp"
#include "app\light_strip\light_strip_controller.hpp"
#include "numerics/math_extensions.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::json;
using namespace mesh::graphics;
using namespace mesh::numerics;
using namespace mesh::app::light_strip::helpers;

namespace mesh::app::light_strip::sources
{
  light_source_type rainbow_source_settings::source_type() const
  {
    return light_source_type::rainbow_source;
  }

  const char* rainbow_source_settings::type_name() const
  {
    return "rainbow";
  }

  std::unique_ptr<json_value> rainbow_source_settings::to_json() const
  {
    auto object = make_unique<json_object>();
    add_type_info(object.get());
    object->set_value("spatialFrequency", spatial_frequency);
    object->set_value("spatialVelocity", angular_velocity);
    return object;
  }

  void rainbow_source_settings::from_json(const json_value* value)
  {
    if (value->type() != json_type::object) return;

    auto object = static_cast<const json_object*>(value);
    object->get_value("spatialFrequency", spatial_frequency);
    object->get_value("spatialVelocity", angular_velocity);
  }

  rainbow_source::rainbow_source()
  {
    rainbow_source_settings default_settings{};
    apply_settings(&default_settings);
  }

  light_source_type rainbow_source::source_type() const
  {
    return light_source_type::rainbow_source;
  }

  void rainbow_source::apply_settings(const light_source_settings* settings)
  {
    auto rainbow_settings = static_cast<const rainbow_source_settings*>(settings);
    spatial_frequency = rainbow_settings->spatial_frequency;
    angular_velocity = rainbow_settings->angular_velocity;
  }

  void rainbow_source::fill(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    _angle += duration_cast<duration<float>>(light_strip_controller::interval).count() * angular_velocity;

    auto angle = deg(_angle);
    auto angle_step = 360.f / pixels.size() * spatial_frequency;
    for(auto& pixel : pixels)
    {
      pixel = color_hsl{ wrap(angle, 0.f, 360.f), 1.f, 0.1f };
      angle += angle_step;
    }
  }
}