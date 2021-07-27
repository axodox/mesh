#include "static_source.hpp"

using namespace std;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::sources
{
  static_source::static_source(light_strip_context& context) :
    light_source(context)
  {
    _properties.steady_frame_source = false;
  }

  light_source_type static_source::type() const
  {
    return light_source_type::static_source;
  }

  void static_source::on_settings_changed()
  {
    _context.frame_ready.set();
  }

  const light_source_settings* static_source::get_settings() const
  {
    return &_context.settings.static_source;
  }

  void static_source::fill(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    for(auto& pixel : pixels)
    {
      pixel = _context.settings.static_source.color;
    }
  }
}