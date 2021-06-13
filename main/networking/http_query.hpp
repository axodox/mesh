#pragma once
#include "esp_http_server.h"

#include "infrastructure/array_view.hpp"

#include "http_query_method.hpp"
#include "http_text_stream.hpp"

namespace mesh::networking
{
  class http_query
  {
  public:
    http_query(httpd_req_t* request);
    ~http_query();
    http_query_method method() const;
    const char* uri() const;

    void return_text(const char* text);
    http_text_stream return_text_stream();

    void return_blob(const char* mime_type, const infrastructure::array_view<uint8_t>& data);

    void return_not_found();

  private:
    httpd_req_t* _request = nullptr;
    bool _has_response = false;

    void check_has_response();
  };
}