#include "text.hpp"

using namespace std;

namespace mesh::infrastructure
{
  std::string format_text(const char* format, va_list args)
  {
    auto size = vsnprintf(nullptr, 0, format, args) + 1;
    string buffer(size, '\0');

    vsnprintf(buffer.data(), size, format, args);
    return buffer;
  }

  std::string format_text(const char* format, ...)
  {
    va_list args;

    va_start(args, format);
    auto result = format_text(format, args);
    va_end(args);
    return result;
  }
}