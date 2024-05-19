#include "empty_source.hpp"

using namespace std;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::sources
{
  empty_source::empty_source(light_strip_context& context) :
    light_source(context)
  {
    _properties.steady_frame_source = false;
    _properties.is_passthrough = true;
  }

  light_source_type empty_source::type() const
  {
    return light_source_type::empty_source;
  }

  const light_source_settings* empty_source::get_settings() const
  {
    return &_context.settings.empty_source;
  }

  void empty_source::fill(std::span<graphics::color_rgb> pixels)
  {
    memset(pixels.data(), 0, pixels.size() * sizeof(graphics::color_rgb));
  }
}