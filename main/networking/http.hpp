#include <string>
#include <functional>
#include <list>

#include "esp_http_server.h"
#include "esp_http_client.h"

#include "infrastructure/logger.hpp"
#include "http_stream.hpp"

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

  class http_query
  {
  public:
    http_query(httpd_req_t* request);
    http_query_method method() const;
    const char* uri() const;
    http_stream stream() const;

  private:
    httpd_req_t* _request; 
  };

  class http_server
  {
    static constexpr infrastructure::logger _logger{"http_server"};

    typedef std::function<void(http_query&)> http_handler_t;

    struct http_handler_data
    {
      http_query_method method;
      const char* uri;
      http_handler_t handler;
    };

  public:
    http_server();
    ~http_server();

    void add_handler(http_query_method method, const char* uri, const http_handler_t& handler);

  private:
    httpd_handle_t _server = nullptr;
    std::list<http_handler_data> _handlers;

    static esp_err_t http_query_handler(httpd_req_t *request);
  };
}