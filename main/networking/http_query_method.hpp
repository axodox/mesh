#pragma once
#include "esp_http_client.h"

namespace mesh::networking
{
  enum class http_query_method
  {
    get = 1,
    post = 3,
    put = 4,
    del = 0
  };

  const char* to_string(http_query_method method);
}