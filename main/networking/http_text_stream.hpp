#pragma once
#include "esp_http_server.h"

namespace mesh::networking
{
  class http_text_stream
  {
  public:
    http_text_stream(httpd_req_t* request);
    ~http_text_stream();

    void print(const char* text);
    void printf(const char* format, ...);
    
  private:
    httpd_req_t* _request;
  };
}