#include "http.hpp"
#include "infrastructure/error.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  esp_err_t test_get_handler(httpd_req_t *request)
  {
    httpd_resp_sendstr_chunk(request, "<!DOCTYPE html><html><body>");
    httpd_resp_sendstr_chunk(request, "Hello World!");
    httpd_resp_sendstr_chunk(request, "</html></body>");
    httpd_resp_sendstr_chunk(request, NULL);
    return ESP_OK;
  }

  http_server::http_server()
  {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    check_result(httpd_start(&_server, &config));

    httpd_uri_t test_query = {
      .uri       = "/*",
      .method    = HTTP_GET,
      .handler   = test_get_handler,
      .user_ctx  = this
    };
    httpd_register_uri_handler(_server, &test_query);
  }

  http_server::~http_server()
  {
    if(_server == nullptr) return;

    httpd_stop(&_server);
    _server = nullptr;
  }
}