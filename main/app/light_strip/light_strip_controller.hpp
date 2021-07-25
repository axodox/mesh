#pragma once
#include <chrono>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include "app/light_strip/sources/light_source.hpp"
#include "app/light_strip/processors/brightness_processor.hpp"
#include "app/light_strip/sources/static_source.hpp"
#include "app/light_strip/sources/rainbow_source.hpp"
#include "peripherals/led_strip.hpp"
#include "networking/http_query.hpp"
#include "infrastructure/task.hpp"
#include "infrastructure/logger.hpp"

namespace mesh::app::light_strip
{
  struct device_settings
  {
    uint32_t light_count = 4;
    std::chrono::milliseconds interval = std::chrono::milliseconds(16);
  };

  struct light_strip_settings
  {
    device_settings device;
    processors::brightness_processor_settings brightness_processor;

    sources::static_source_settings static_source;
    sources::rainbow_source_settings rainbow_source;
  };

  class light_strip_controller
  {
    static constexpr infrastructure::logger _logger{"light_strip_controller"};
    inline static const char * _root_uri = "/api/light_strip/*";
    inline static const char * _mode_uri = "/api/light_strip/mode";
    inline static const char * _brightness_uri = "/api/light_strip/brightness";

  public:
    static const std::chrono::milliseconds interval;

    light_strip_controller();
    ~light_strip_controller();

  private:    
    bool _isDisposed = false;
    uint32_t _light_count;
    std::shared_ptr<peripherals::led_strip> _strip;
    std::unique_ptr<sources::light_source> _source;
    std::unique_ptr<processors::brightness_processor> _brightness_processor;
    std::mutex _mutex;
    infrastructure::task _thread;

    void worker();
    void on_get(networking::http_query &query);
    void on_post(networking::http_query &query);

    void on_post_brightness(networking::http_query &query);
    void on_post_mode(networking::http_query &query);
  };
}