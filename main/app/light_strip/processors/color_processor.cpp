#include "color_processor.hpp"

namespace mesh::app::light_strip::processors
{
  color_processor::color_processor(const settings::light_strip_settings* settings)
  {
    _settings = settings;
  }
}