#include "http.hpp"
#include "infrastructure/error.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  const char* to_string(http_query_method method)
  {
    switch (method)
    {
    case http_query_method::get:
      return "GET";
    case http_query_method::post:
      return "POST";
    case http_query_method::put:
      return "PUT";
    case http_query_method::del:
      return "DELETE";
    default:
      return "UNKNOWN";
    }
  }

  http_query::http_query(httpd_req_t* request) :
    _request(request)
  { }

  http_query_method http_query::method() const
  {
    return (http_query_method)_request->method;
  }

  const char* http_query::uri() const
  {
    return _request->uri;
  }

  http_stream http_query::stream() const
  {
    return http_stream(_request);
  }

  http_server::http_server()
  {
    _logger.log_message(log_severity::info, "Starting...");
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    check_result(httpd_start(&_server, &config));
    _logger.log_message(log_severity::info, "Started.");
  }

  http_server::~http_server()
  {
    if(_server == nullptr) return;

    _logger.log_message(log_severity::info, "Stopping...");
    httpd_stop(&_server);
    _server = nullptr;
    _logger.log_message(log_severity::info, "Stopped.");
  }

  void http_server::add_handler(http_query_method method, const char* uri, const http_handler_t& handler)
  {
    _handlers.push_back({});
    auto& handler_data = _handlers.back();
    
    handler_data.method = method;
    handler_data.uri = uri;
    handler_data.handler = handler;
    
    httpd_uri_t uri_registration = {
      .uri = uri,
      .method = (httpd_method_t)method,
      .handler = http_server::http_query_handler,
      .user_ctx = &handler_data
    };
    check_result(httpd_register_uri_handler(_server, &uri_registration));

    _logger.log_message(log_severity::info, "Added handler: %s %s", to_string(method), uri);
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