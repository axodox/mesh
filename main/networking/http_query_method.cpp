#include "http_query_method.hpp"

namespace mesh::networking
{
  const char* to_string(http_query_method method)
  {
    switch (method)
    {
    case http_query_method::get:
      return "GET";
    case http_query_method::post:
      return "POST";
    case http_query_method::put:
      return "PUT";
    case http_query_method::del:
      return "DELETE";
    default:
      return "UNKNOWN";
    }
  }
}