#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  light_source::light_source(light_strip_context& context) :
    _context(context)
  { }

  const light_source_properties& light_source::properties() const
  {
    return _properties;
  }
}