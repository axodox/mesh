#pragma once
#include <chrono>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <optional>

#include "peripherals/led_strip.hpp"
#include "networking/http_query.hpp"
#include "threading/task.hpp"
#include "infrastructure/logger.hpp"

#include "app/light_strip/light_strip_context.hpp"
#include "app/light_strip/sources/light_source.hpp"
#include "app/light_strip/sources/empty_source.hpp"
#include "app/light_strip/sources/static_source.hpp"
#include "app/light_strip/sources/rainbow_source.hpp"
#include "app/light_strip/sources/udp_source.hpp"
#include "app/light_strip/processors/brightness_processor.hpp"
#include "app/light_strip/settings/light_strip_settings.hpp"

namespace mesh::app::light_strip
{
  class light_strip_controller : public light_strip_context
  {
    static constexpr infrastructure::logger _logger{"light_strip_controller"};
    inline static const char * _root_uri = "/api/light_strip/*";
    inline static const char * _mode_uri = "/api/light_strip/mode";
    inline static const char * _brightness_uri = "/api/light_strip/brightness";
    inline static const char * _device_uri = "/api/light_strip/device";
    inline static const char * _settings_path = "/data/light_strip.json";
    inline static const std::chrono::seconds _settings_save_delay = std::chrono::seconds(10);

  public:
    light_strip_controller();
    ~light_strip_controller();

  private:    
    bool _isDisposed = false;
    std::shared_ptr<peripherals::led_strip> _strip;
    std::unique_ptr<sources::light_source> _source;
    std::unique_ptr<processors::brightness_processor> _brightness_processor;
    std::mutex _mutex;
    std::unique_ptr<threading::task> _thread;    
    std::optional<std::chrono::steady_clock::time_point> _last_settings_change;

    void worker();
    void on_get(networking::http_query &query);
    void on_post(networking::http_query &query);

    void on_post_brightness(networking::http_query &query);
    void on_post_mode(networking::http_query &query);
    void on_post_device(networking::http_query &query);

    void load_settings();
    void save_settings();

    void initialize_source();
  };
}