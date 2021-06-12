#pragma once
#include <string>
#include <functional>
#include <list>

#include "esp_http_server.h"
#include "esp_http_client.h"

#include "infrastructure/logger.hpp"
#include "http_query.hpp"

namespace mesh::networking
{
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