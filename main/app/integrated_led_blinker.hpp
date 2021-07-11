#pragma once
#include <memory>
#include "peripherals/integrated_led.hpp"
#include "networking/http_query.hpp"

namespace mesh::app
{
  class integrated_led_blinker
  {
  public:
    integrated_led_blinker();

  private:
    std::shared_ptr<peripherals::integrated_led> _led;

    void on_api_call(networking::http_query &query);
  };
}