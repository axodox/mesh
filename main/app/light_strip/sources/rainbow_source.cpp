#include "rainbow_source.hpp"
#include "app\light_strip\light_strip_controller.hpp"
#include "numerics/math_extensions.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::graphics;
using namespace mesh::numerics;

namespace mesh::app::light_strip::sources
{
  light_source_type rainbow_source_settings::type() const
  {
    return light_source_type::rainbow_source;
  }

  light_source_type rainbow_source::type() const
  {
    return light_source_type::rainbow_source;
  }

  void rainbow_source::on_device_settings_changed(const device_settings& settings)
  {
    _interval = duration_cast<duration<float>>(settings.interval);
  }

  const light_source_settings * rainbow_source::get_settings() const
  {
    return &_settings;
  }

  void rainbow_source::apply_settings(const light_source_settings* settings)
  {
    _settings = static_cast<const rainbow_source_settings&>(*settings);
  }

  void rainbow_source::fill(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    _angle = wrap(_angle + _interval.count() * deg(_settings.angular_velocity), 0.f, 360.f);

    auto angle = _angle;
    auto angle_step = 360.f / pixels.size() * _settings.spatial_frequency;
    for(auto& pixel : pixels)
    {
      pixel = color_hsl{ wrap(angle, 0.f, 360.f), 1.f, 0.1f };
      angle += angle_step;
    }
  }
}