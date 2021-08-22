#pragma once

#include <mutex>
#include <thread>
#include <string>

#include "esp_http_server.h"
#include "infrastructure/events.hpp"

namespace mesh::networking
{
  class http_server;

  class http_socket
  {
    friend class http_server;
    events::event_owner _events;

  public:
    http_socket(http_server* server, httpd_req_t* request);
    ~http_socket();

    bool operator ==(const http_socket& other) const;
    http_server* server() const;

    events::event_publisher<http_socket*, const char*> message_received;
    events::event_publisher<http_socket*> closing;

    void send_message(const char* message);

  private:
    http_server* _server;
    httpd_req_t* _request;
    std::recursive_mutex _mutex;
    std::thread::id _receive_thread_id = {};
    httpd_handle_t _server_handle;
    int _socket_descriptor = 0;

    void on_receive(httpd_req_t* request);
  };
}