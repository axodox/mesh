#include "integrated_led.hpp"
#include "driver/gpio.h"

namespace mesh::peripherals
{
  integrated_led::integrated_led(uint8_t pin) :
    _pin(pin)
  {
    gpio_reset_pin(gpio_num_t(_pin));
    gpio_set_direction(gpio_num_t(_pin), GPIO_MODE_OUTPUT);
    gpio_set_level(gpio_num_t(_pin), 0);
  }
  
  bool integrated_led::state() const
  {
    return _state;
  }

  void integrated_led::state(bool value)
  {
    if(_state == value) return;

    _state = value;
    gpio_set_level(gpio_num_t(_pin), _state);
  }
}