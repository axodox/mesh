#pragma once
#include <string>
#include <cstring>
#include <cstdarg>

namespace mesh::infrastructure
{
  std::string format_text(const char* format, va_list args);

  std::string format_text(const char* format, ...);

  struct cstring_comparer {
    bool operator()(const char* a, const char* b) const {
      return strcmp(a, b) < 0;
    }
  };
}