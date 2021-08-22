#include "http_socket.hpp"
#include "http_server.hpp"
#include "infrastructure/error.hpp"

using namespace mesh::infrastructure;
using namespace std;

namespace mesh::networking
{
  thread_local httpd_req_t* _request;

  http_socket::http_socket(http_server* server, httpd_req_t* request) :
    message_received(_events),
    closing(_events),
    _server(server)
  {
    _server_handle = request->handle;
    _socket_descriptor = httpd_req_to_sockfd(request);
  }

  http_socket::~http_socket()
  {
    _events.raise(closing, this);
  }

  http_server* http_socket::server() const
  {
    return _server;
  }

  bool http_socket::operator ==(const http_socket& other) const
  {
    return this == &other;
  }

  void http_socket::on_receive(httpd_req_t* request)
  {
    //begin
    _request = request;

    //receive
    httpd_ws_frame_t frame;
    memset(&frame, 0, sizeof(httpd_ws_frame_t));
    frame.type = HTTPD_WS_TYPE_TEXT;
    check_result(httpd_ws_recv_frame(request, &frame, 0));

    if(frame.len > 0)
    {
      std::string message('\0', frame.len);
      frame.payload = reinterpret_cast<uint8_t*>(message.data());
      check_result(httpd_ws_recv_frame(request, &frame, frame.len));

      _events.raise(message_received, this, message.data());
    }

    //end
    _request = nullptr;
  }

  void http_socket::send_message(const char* message)
  {
    httpd_ws_frame_t frame;
    memset(&frame, 0, sizeof(httpd_ws_frame_t));
    frame.type = HTTPD_WS_TYPE_TEXT;
    frame.len = strlen(message);
    frame.payload = reinterpret_cast<uint8_t*>(const_cast<char*>(message));

    if(_request)
    {
      //send sync      
      check_result(httpd_ws_send_frame(_request, &frame));
    }
    else
    {
      //send async
      check_result(httpd_ws_send_frame_async(_server_handle, _socket_descriptor, &frame));
    }
  }
}