#include "rainbow_source.hpp"
#include "app\light_strip\light_strip_controller.hpp"
#include "numerics/math_extensions.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::graphics;
using namespace mesh::numerics;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::sources
{
  light_source_type rainbow_source::type() const
  {
    return light_source_type::rainbow_source;
  }

  const light_source_settings* rainbow_source::get_settings() const
  {
    return &_context.settings.rainbow_source;
  }

  void rainbow_source::fill(std::span<graphics::color_rgb> pixels)
  {
    _angle = wrap(_angle + duration_cast<duration<float>>(_context.settings.device.interval).count() * deg(_context.settings.rainbow_source.angular_velocity), 0.f, 360.f);

    auto angle = _angle * _context.settings.rainbow_source.spatial_frequency;
    auto angle_step = 360.f / pixels.size() * _context.settings.rainbow_source.spatial_frequency;
    for(auto& pixel : pixels)
    {
      pixel = color_hsl{ wrap(angle, 0.f, 360.f), 1.f, 0.5f };
      angle += angle_step;
    }
  }
}