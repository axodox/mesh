#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  light_source::light_source(light_strip_context& context) :
    _context(context)
  { }

  std::span<const uint8_t> light_source::gains()
  {
    return {};
  }
}