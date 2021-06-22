#pragma once
#include <string>
#include <functional>
#include <vector>

#include "esp_http_server.h"
#include "esp_http_client.h"

#include "infrastructure/array_view.hpp"
#include "infrastructure/logger.hpp"
#include "mime_type.hpp"
#include "http_query.hpp"

namespace mesh::networking
{
  enum class http_static_file_options
  {
    none,
    compress_gzip,
    redirect_root
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

    struct http_static_file
    {
      const char* uri;
      const char* mime_type;
      infrastructure::array_view<uint8_t> data;
      http_static_file_options options;
    };

  public:
    http_server() = default;
    ~http_server();

    void start();
    void stop();

    void add_handler(http_query_method method, const char* uri, const http_handler_t& handler);

    void add_static_file(const char * uri, const char* mime_type, const infrastructure::array_view<uint8_t>& data, http_static_file_options options = {});

  private:
    httpd_handle_t _server = nullptr;
    std::vector<http_handler_data> _handlers;
    std::vector<http_static_file> _files;

    static esp_err_t http_query_handler(httpd_req_t *request);
    static esp_err_t http_static_file_handler(httpd_req_t *request);
  };
}