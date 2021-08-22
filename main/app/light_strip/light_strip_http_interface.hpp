#pragma once
#include <list>
#include "light_strip_controller.hpp"
#include "networking/http_query.hpp"
#include "networking/http_socket.hpp"
#include "serialization/json/json_container_serializer.hpp"

namespace mesh::app::light_strip
{
  class light_strip_http_interface
  {
    static constexpr infrastructure::logger _logger{"light_strip_http_interface"};
    inline static const char * _root_uri = "/api/light_strip/*";
    inline static const char * _mode_uri = "/api/light_strip/mode";
    inline static const char * _brightness_uri = "/api/light_strip/brightness";
    inline static const char * _device_uri = "/api/light_strip/device";
    inline static const char * _socket_uri = "/ws/light_strip";

    inline static const char * _brightness_key = "brightness";
    inline static const char * _source_key = "source";
    inline static const char * _device_key = "device";

  public:
    light_strip_http_interface();

  private:
    std::shared_ptr<light_strip_controller> _controller;
    std::list<networking::http_socket*> _sockets;
    std::mutex _mutex;
    serialization::json::json_container_serializer _message_serializer;

    void on_get(networking::http_query &query);
    void on_post(networking::http_query &query);
    void on_socket(networking::http_socket &socket);

    void on_post_brightness(networking::http_query &query);
    void on_post_mode(networking::http_query &query);
    void on_post_device(networking::http_query &query);

    void on_socket_received(networking::http_socket* socket, const char* message);
    void on_socket_closing(networking::http_socket* socket);

    void apply_source_settings(const settings::light_source_settings* source_settings);
  };
}