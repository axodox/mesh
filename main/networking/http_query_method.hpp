#pragma once
#include "esp_http_client.h"

namespace mesh::networking
{
  enum class http_query_method
  {
    get = HTTP_METHOD_GET,
    post = HTTP_METHOD_POST,
    put = HTTP_METHOD_PUT,
    del = HTTP_METHOD_DELETE
  };

  const char* to_string(http_query_method method);
}