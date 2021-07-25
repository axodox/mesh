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

  void http_query::set_header(const char* field, const char* value)
  {
    check_has_response(false);
    check_result(httpd_resp_set_hdr(_request, field, value));
  }

  void http_query::return_text(const char* text)
  {
    check_has_response();
    check_result(httpd_resp_sendstr(_request, text));
  }

  void http_query::return_text(const std::string text)
  {
    return_text(text.c_str());
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
    check_result(httpd_resp_send(_request, reinterpret_cast<const char*>(data.data()), ssize_t(data.size())));
  }

  void http_query::return_not_found()
  {
    check_has_response();
    check_result(httpd_resp_send_404(_request));
  }

  const infrastructure::array_view<uint8_t> http_query::get_body()
  {
    if(_body.empty() && _request->content_len > 0)
    {
      _body.resize(_request->content_len + 1);
      auto length = httpd_req_recv(_request, reinterpret_cast<char*>(_body.data()), _body.size());
      _body.resize(length + 1);
      _body.back() = 0; //Add zero termination for strings
    }

    return { _body.data(), _body.data() + _request->content_len };
  }

  const std::string_view http_query::get_text()
  {
    return { reinterpret_cast<const char*>(get_body().data()) };
  }

  void http_query::check_has_response(bool set_response)
  {
    if(_has_response)
    {
      throw std::logic_error("This HTTP query has a response already!");
    }
    else if(set_response)
    {
      _has_response = true;
    }
  }
}