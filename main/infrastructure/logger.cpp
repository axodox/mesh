#include "logger.hpp"
#include "text.hpp"

using namespace std;

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

  std::string get_timestamp()
  {
    time_t utc_now;
    time(&utc_now);

    tm local_now;
    localtime_r(&utc_now, &local_now);

    string time_as_text(9, '\0');
    strftime(time_as_text.data(), time_as_text.size(), "%T", &local_now);

    return time_as_text;
  }

  void log_message(log_severity severity, const char *format, ...)
  {
    va_list args;
    va_start(args, format);
    auto text = format_text("%s [%s] main: %s\n", get_timestamp().c_str(), to_string(severity), format_text(format, args).c_str());
    va_end(args);
    printf(text.c_str());
  }

  void logger::log_message(log_severity severity, const char *format, ...) const
  {
    va_list args;
    va_start(args, format);
    auto text = format_text("%s [%s] %s: %s\n", get_timestamp().c_str(), to_string(severity), _name, format_text(format, args).c_str());
    va_end(args);
    printf(text.c_str());
  }
}