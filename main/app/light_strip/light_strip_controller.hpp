#pragma once
#include <chrono>
#include <memory>
#include <thread>
#include <mutex>
#include "app/light_strip/sources/light_source.hpp"
#include "peripherals/led_strip.hpp"
#include "networking/http_query.hpp"
#include "infrastructure/task.hpp"
#include "infrastructure/logger.hpp"

namespace mesh::app::light_strip
{
  class light_strip_controller
  {
    static constexpr infrastructure::logger _logger{"light_strip_controller"};
    
  public:
    static const std::chrono::milliseconds interval;

    light_strip_controller();
    ~light_strip_controller();

  private:
    bool _isDisposed = false;
    uint32_t _light_count;
    std::shared_ptr<peripherals::led_strip> _strip;
    std::unique_ptr<sources::light_source> _source;
    std::mutex _mutex;
    infrastructure::task _thread;

    void worker();
    void on_post(networking::http_query &query);
  };
}