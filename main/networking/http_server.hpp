#pragma once
#include <string>
#include <functional>
#include <vector>
#include <list>

#include "esp_http_server.h"
#include "esp_http_client.h"

#include "infrastructure/array_view.hpp"
#include "infrastructure/logger.hpp"
#include "mime_type.hpp"
#include "http_query.hpp"
#include "http_socket.hpp"

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

    typedef std::function<void(http_query&)> http_query_handler_t;
    typedef std::function<void(http_socket&)> http_socket_handler_t;

    struct http_query_handler_data
    {
      http_query_method method;
      const char* uri;
      http_query_handler_t handler;
    };

    struct http_static_file
    {
      const char* uri;
      const char* mime_type;
      infrastructure::array_view<uint8_t> data;
      http_static_file_options options;
    };

    struct http_socket_handler_data
    {
      const char* uri;
      const char* protocol;
      http_socket_handler_t handler;
    };

  public:
    http_server() = default;
    ~http_server();

    void start();
    void stop();

    void add_handler(http_query_method method, const char* uri, const http_query_handler_t& handler);
    void add_static_file(const char * uri, const char* mime_type, const infrastructure::array_view<uint8_t>& data, http_static_file_options options = {});
    void add_socket(const char* uri, const http_socket_handler_t& handler, const char* protocol = nullptr);

  private:
    httpd_handle_t _server = nullptr;
    std::vector<http_query_handler_data> _query_handlers;
    std::vector<http_static_file> _files;
    std::vector<http_socket_handler_data> _socket_handlers;
    std::list<http_socket> _sockets;

    static esp_err_t http_query_handler(httpd_req_t *request);
    static esp_err_t http_static_file_handler(httpd_req_t *request);
    static esp_err_t http_socket_handler(httpd_req_t *request);
    static void close_socket(void* context);
  };
}