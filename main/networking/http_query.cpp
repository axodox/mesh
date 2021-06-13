#include "infrastructure/error.hpp"
#include "http_query.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  http_query::http_query(httpd_req_t* request) :
    _request(request)
  { }

  http_query::~http_query()
  {
    if(!_has_response)
    {
      check_result(httpd_resp_send(_request, nullptr, 0));
    }
  }

  http_query_method http_query::method() const
  {
    return (http_query_method)_request->method;
  }

  const char* http_query::uri() const
  {
    return _request->uri;
  }

  void http_query::return_text(const char* text)
  {
    check_has_response();
    check_result(httpd_resp_sendstr(_request, text));
  }

  http_text_stream http_query::return_text_stream()
  {
    check_has_response();
    return http_text_stream(_request);
  }

  void http_query::return_blob(const char* mime_type, const infrastructure::array_view<uint8_t>& data)
  {
    check_has_response();
    check_result(httpd_resp_set_type(_request, mime_type));
    check_result(httpd_resp_send(_request, reinterpret_cast<const char*>(data.begin()), ssize_t(data.size())));
  }

  void http_query::check_has_response()
  {
    if(_has_response)
    {
      throw std::logic_error("This HTTP query has a response already!");
    }
    else
    {
      _has_response = true;
    }
  }
}