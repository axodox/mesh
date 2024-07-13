#pragma once
#include "infrastructure/logger.hpp"
#include "esp_sntp.h"

namespace mesh::networking
{
  class ntp_client
  {
    static constexpr infrastructure::logger _logger{"ntp_client"};

  public:
    ntp_client(const char* server = "pool.ntp.org");

    bool is_in_sync() const;

    static void set_timezone(const char* timezone);

  private:
    inline static const char* _timezone = "CEST-1CET,M3.2.0/2:00:00,M11.1.0/2:00:00";
    static void on_time_synchronized(timeval *time);
  };
}