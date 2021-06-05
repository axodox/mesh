#pragma once
#include <string>
#include <cstdarg>

namespace mesh::infrastructure
{
  std::string format_text(const char* format, va_list args);

  std::string format_text(const char* format, ...);
}