#include "logger.hpp"
#include "text.hpp"

namespace mesh::infrastructure
{
  const char* to_string(log_severity severity)
  {
    switch(severity)
    {
      case log_severity::debug:
        return "debug";
      case log_severity::info:
        return "info";
      case log_severity::warning:
        return "warning";
      case log_severity::error:
        return "error";
      case log_severity::fatal:
        return "fatal";
      default:
        return "unknown";
    }
  }

  void log_message(log_severity severity, const char *format, ...)
  {
    va_list arg;
    auto text = format_text(">> %s: %s\n", to_string(severity), format_text(format, arg).c_str());
    printf(text.c_str());
  }

  void logger::log_message(log_severity severity, const char *format, ...) const
  {
    va_list arg;
    auto text = format_text(">> %s: %s - %s\n", to_string(severity), _name, format_text(format, arg).c_str());
    printf(text.c_str());
  }
}