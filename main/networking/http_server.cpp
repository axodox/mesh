#include "http_server.hpp"
#include <algorithm>
#include "infrastructure/bitwise_operations.hpp"
#include "infrastructure/error.hpp"

using namespace std;
using namespace mesh::infrastructure;

namespace mesh::networking
{
  http_server::~http_server()
  {
    if (_server) 
    {
      stop();
    }
  }

  void http_server::start()
  {
    if (_server)
    {
      _logger.log_message(log_severity::warning, "The server is already running!");
      return;
    }

    //Create server
    _logger.log_message(log_severity::info, "Starting...");
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.max_uri_handlers = uint16_t(_handlers.size()) + !_files.empty();
    config.lru_purge_enable = true;
    check_result(httpd_start(&_server, &config));

    //Custom handlers
    for (auto &handler : _handlers)
    {
      httpd_uri_t uri_registration = {
        .uri = handler.uri,
        .method = (httpd_method_t)handler.method,
        .handler = http_server::http_query_handler,
        .user_ctx = &handler
      };
      check_result(httpd_register_uri_handler(_server, &uri_registration));

      _logger.log_message(log_severity::info, "Added handler: %s %s", to_string(handler.method), handler.uri);
    }

    //Serve static files
    if (!_files.empty())
    {
      httpd_uri_t uri_registration = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = http_server::http_static_file_handler,
        .user_ctx = this
      };
      check_result(httpd_register_uri_handler(_server, &uri_registration));

      _logger.log_message(log_severity::info, "Added static files handler.");
    }

    _logger.log_message(log_severity::info, "Started.");
  }

  void http_server::stop()
  {
    if (!_server)
    {
      _logger.log_message(log_severity::warning, "The server is already stopped!");
      return;
    }

    _logger.log_message(log_severity::info, "Stopping...");
    httpd_stop(&_server);
    _server = nullptr;
    _logger.log_message(log_severity::info, "Stopped.");
  }

  void http_server::add_handler(http_query_method method, const char *uri, const http_handler_t &handler)
  {
    if (_server)
    {
      throw std::logic_error("Cannot add HTTP handlers after the server is started!");
    }

    _handlers.push_back({});
    auto &handler_data = _handlers.back();

    handler_data.method = method;
    handler_data.uri = uri;
    handler_data.handler = handler;
  }

  void http_server::add_static_file(const char *uri, const char *mime_type, const infrastructure::array_view<uint8_t> &data, http_static_file_options options)
  {
    if (_server)
    {
      throw std::logic_error("Cannot add HTTP static files after the server is started!");
    }

    {
      _files.push_back(http_static_file());
      auto &file = _files.back();
      file.uri = uri;
      file.mime_type = mime_type;
      file.data = data;
      file.options = options;
    }

    if (has_flag(options, http_static_file_options::redirect_root))
    {
      _files.push_back(http_static_file());
      auto &file = _files.back();
      file.uri = "/";
      file.mime_type = mime_type;
      file.data = data;
      file.options = options;
    }
  }

  esp_err_t http_server::http_query_handler(httpd_req_t *request)
  {
    auto handler_data = (http_handler_data *)request->user_ctx;
    http_query query(request);

    _logger.log_message(log_severity::info, "Serving %s %s...", to_string(query.method()), query.uri());

    try
    {
      handler_data->handler(query);
      return ESP_OK;
    }
    catch (const std::exception &e)
    {
      _logger.log_message(log_severity::error, "Query %s failed. Reason: %s", handler_data->uri, e.what());
      return ESP_FAIL;
    }

    return ESP_OK;
  }

  esp_err_t http_server::http_static_file_handler(httpd_req_t *request)
  {
    auto server = (http_server *)request->user_ctx;

    auto it = find_if(server->_files.begin(), server->_files.end(), [&](const http_static_file &file) { return strcmp(file.uri, request->uri) == 0; });

    http_query query{request};
    if (it != server->_files.end())
    {
      _logger.log_message(log_severity::info, "Serving file %s...", request->uri);

      auto &file = *it;
      if (has_flag(file.options, http_static_file_options::compress_gzip))
      {
        query.set_header("Content-Encoding", "gzip");
      }

      query.return_blob(file.mime_type, file.data);
    }
    else
    {
      _logger.log_message(log_severity::warning, "Missing file %s.", request->uri);
      query.return_not_found();
    }

    return ESP_OK;
  }
}