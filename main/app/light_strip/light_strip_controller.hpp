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
  struct light_strip_settings
  {
    sources::device_settings device;
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
    inline static const char * _device_uri = "/api/light_strip/device";

  public:
    light_strip_controller();
    ~light_strip_controller();

    const sources::device_settings& get_settings() const;

  private:    
    bool _isDisposed = false;
    sources::device_settings _settings{};
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
    void on_post_device(networking::http_query &query);
  };
}