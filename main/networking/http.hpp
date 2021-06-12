#include "esp_http_server.h"

namespace mesh::networking
{
  class http_server
  {
  public:
    http_server();
    ~http_server();

  private:
    httpd_handle_t _server = nullptr;
  };
}