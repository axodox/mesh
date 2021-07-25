#include "static_source.hpp"

using namespace std;

namespace mesh::app::light_strip::sources
{
  light_source_type static_source_settings::type() const
  {
    return light_source_type::static_source;
  }

  light_source_type static_source::type() const
  {
    return light_source_type::static_source;
  }

  const light_source_settings * static_source::get_settings() const
  {
    return &_settings;
  }

  void static_source::apply_settings(const light_source_settings* settings)
  {
    _settings = static_cast<const static_source_settings&>(*settings);
  }

  void static_source::fill(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    for(auto& pixel : pixels)
    {
      pixel = _settings.color;
    }
  }
}