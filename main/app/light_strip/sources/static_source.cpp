#include "static_source.hpp"

using namespace std;

namespace mesh::app::light_strip::sources
{
  light_source_type static_source_settings::type() const
  {
    return light_source_type::static_source;
  }

  static_source::static_source()
  {
    static_source_settings default_settings{};
    apply_settings(&default_settings);
  }

  light_source_type static_source::type() const
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