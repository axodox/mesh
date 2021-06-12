#include "http_stream.hpp"
#include "infrastructure/error.hpp"
#include "infrastructure/text.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  http_stream::http_stream(httpd_req_t* request) :
    _request(request)
  { }

  http_stream::~http_stream()
  {
    check_result(httpd_resp_sendstr_chunk(_request, nullptr));
  }

  void http_stream::print(const char* text)
  {
    check_result(httpd_resp_sendstr_chunk(_request, text));
  }

  void http_stream::printf(const char* format, ...)
  {
    va_list args;

    va_start(args, format);
    auto text = format_text(format, args);
    va_end(args);
    print(text.c_str());
  }
}