#include "embedded.hpp"

namespace mesh::storage::embedded
{
  embedded_file::operator const infrastructure::array_view<uint8_t>() const
  {
    return infrastructure::array_view<uint8_t>(start, end);
  }
}