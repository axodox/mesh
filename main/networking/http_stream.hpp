#include "esp_http_server.h"

namespace mesh::networking
{
  class http_stream
  {
  public:
    http_stream(httpd_req_t* request);
    ~http_stream();

    void print(const char* text);
    void printf(const char* format, ...);

  private:
    httpd_req_t* _request;
  };
}