#include "static_source.hpp"

using namespace std;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::sources
{
  light_source_type static_source::type() const
  {
    return light_source_type::static_source;
  }

  const light_source_settings* static_source::get_settings() const
  {
    return &_settings->static_source;
  }

  void static_source::fill(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    for(auto& pixel : pixels)
    {
      pixel = _settings->static_source.color;
    }
  }
}