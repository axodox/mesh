#pragma once
#include "esp_http_client.h"

namespace mesh::networking
{
  enum class http_query_method
  {
    get = HTTP_GET,
    post = HTTP_POST,
    put = HTTP_PUT,
    del = HTTP_DELETE
  };

  const char* to_string(http_query_method method);
}