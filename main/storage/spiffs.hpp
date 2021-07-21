#pragma once
#include "infrastructure/logger.hpp"

namespace mesh::storage
{
  class spiffs
  {
    static constexpr infrastructure::logger _logger{"spiffs"};

  public:
    spiffs(const char* root = "/data", size_t max_open_files = 5);
    ~spiffs();

  private:
    const char* _partition_id;
  };
}