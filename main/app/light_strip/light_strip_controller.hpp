#pragma once
#include <chrono>
#include <memory>
#include <thread>
#include "app/light_strip/sources/light_source.hpp"
#include "peripherals/led_strip.hpp"

namespace mesh::app::light_strip
{
  class light_strip_controller
  {
  public:
    light_strip_controller();
    ~light_strip_controller();

  private:    
    bool _isDisposed = false;
    std::chrono::milliseconds _interval;
    uint32_t _light_count;
    std::shared_ptr<peripherals::led_strip> _strip;
    std::unique_ptr<sources::light_source> _source;
    std::thread _thread;

    void worker();
  };
}