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
    static const std::chrono::milliseconds interval;

    light_strip_controller();
    ~light_strip_controller();

  private:
    bool _isDisposed = false;
    uint32_t _light_count;
    std::shared_ptr<peripherals::led_strip> _strip;
    std::unique_ptr<sources::light_source> _source;
    std::thread _thread;

    void worker();
  };
}