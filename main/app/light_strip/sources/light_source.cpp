#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  light_source::light_source(const settings::light_strip_settings* settings)
  {
    _settings = settings;
  }
}