#pragma once
#include <cstdint>

namespace mesh::peripherals
{
  class integrated_led
  {
  public:
    integrated_led(uint8_t pin = 2);
    bool state() const;
    void state(bool value);

  private:
    uint8_t _pin;
    bool _state;
  };
}