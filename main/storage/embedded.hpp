#pragma once
#include "infrastructure/array_view.hpp"

namespace mesh::storage::embedded
{
  struct embedded_file
  {
    const uint8_t* start;
    const uint8_t* end;

    operator const infrastructure::array_view<uint8_t>() const;
  };
}

#define define_file(var_name, start, end) namespace _files { \
  extern const uint8_t start_##var_name[] __asm(start); \
  extern const uint8_t end_##var_name[] __asm(end); \
} mesh::storage::embedded::embedded_file var_name { _files::start_##var_name, _files::end_##var_name }
