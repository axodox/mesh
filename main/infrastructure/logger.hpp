#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "esp_log.h"

namespace mesh::infrastructure
{
  enum class log_severity : uint8_t
  {
    debug,
    info,
    warning,
    error,
    fatal
  };

  void log_message(log_severity severity, const char *format, ...);

  class logger
  {
  public:
    constexpr logger(const char* name) : _name(name) {}
    void log_message(log_severity severity, const char *format, ...) const;

  private:
    const char* _name;
  };
}