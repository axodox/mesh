#pragma once
#include <cstdint>

#ifdef INTEGRATED_LED_PIN
#define INTEGRATED_LED_DEFAULT_PIN = INTEGRATED_LED_PIN
#else
#define INTEGRATED_LED_DEFAULT_PIN
#endif

namespace mesh::peripherals
{
  class integrated_led
  {
  public:
    integrated_led(uint8_t pin INTEGRATED_LED_DEFAULT_PIN);
    bool state() const;
    void state(bool value);

  private:
    uint8_t _pin;
    bool _state;
  };
}