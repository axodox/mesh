#include "http_text_stream.hpp"
#include "infrastructure/error.hpp"
#include "infrastructure/text.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  http_text_stream::http_text_stream(httpd_req_t* request) :
    _request(request)
  { }

  http_text_stream::~http_text_stream()
  {
    check_result(httpd_resp_sendstr_chunk(_request, nullptr));
  }

  void http_text_stream::print(const char* text)
  {
    check_result(httpd_resp_sendstr_chunk(_request, text));
  }

  void http_text_stream::printf(const char* format, ...)
  {
    va_list args;

    va_start(args, format);
    auto text = format_text(format, args);
    va_end(args);
    print(text.c_str());
  }
}