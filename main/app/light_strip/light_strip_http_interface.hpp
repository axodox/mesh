#pragma once
#include "light_strip_controller.hpp"

namespace mesh::app::light_strip
{
  class light_strip_http_interface
  {
    static constexpr infrastructure::logger _logger{"light_strip_http_interface"};
    inline static const char * _root_uri = "/api/light_strip/*";
    inline static const char * _mode_uri = "/api/light_strip/mode";
    inline static const char * _brightness_uri = "/api/light_strip/brightness";
    inline static const char * _device_uri = "/api/light_strip/device";

  public:
    light_strip_http_interface();

  private:
    std::shared_ptr<light_strip_controller> _controller;

    void on_get(networking::http_query &query);
    void on_post(networking::http_query &query);

    void on_post_brightness(networking::http_query &query);
    void on_post_mode(networking::http_query &query);
    void on_post_device(networking::http_query &query);
  };
}