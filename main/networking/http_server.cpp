#include "http_server.hpp"
#include "infrastructure/error.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  http_server::~http_server()
  {
    if(_server) stop();
  }

  void http_server::start()
  {
    if(_server)
    {
      _logger.log_message(log_severity::warning, "The server is already running!");
      return;
    }

    _logger.log_message(log_severity::info, "Starting...");
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.max_uri_handlers = uint16_t(_handlers.size());
    config.lru_purge_enable = true;
    check_result(httpd_start(&_server, &config));

    for(auto& handler : _handlers)
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
    _logger.log_message(log_severity::info, "Started.");
  }

  void http_server::stop()
  {
    if(!_server)
    {
      _logger.log_message(log_severity::warning, "The server is already stopped!");
      return;
    }

    _logger.log_message(log_severity::info, "Stopping...");
    httpd_stop(&_server);
    _server = nullptr;
    _logger.log_message(log_severity::info, "Stopped.");
  }

  void http_server::add_handler(http_query_method method, const char* uri, const http_handler_t& handler)
  {
    if(_server)
    {
      throw std::logic_error("Cannot add HTTP handlers after the server is started!");
    }

    _handlers.push_back({});
    auto& handler_data = _handlers.back();
    
    handler_data.method = method;
    handler_data.uri = uri;
    handler_data.handler = handler;
  }

  esp_err_t http_server::http_query_handler(httpd_req_t *request)
  {
    auto handler_data = (http_handler_data*)request->user_ctx;
    http_query query(request);

    _logger.log_message(log_severity::info, "Serving %s %s...", to_string(query.method()), query.uri());

    try
    {
      handler_data->handler(query);
      return ESP_OK;
    }
    catch(const std::exception& e)
    {
      _logger.log_message(log_severity::error, "Query %s failed. Reason: %s", handler_data->uri, e.what());
      return ESP_FAIL;
    }

    return ESP_OK;
  }
}