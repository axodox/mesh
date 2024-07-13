#pragma once
#include <chrono>

#include "hwconfig.h"
#include "led_strip.hpp"

#ifdef WS281X_PIN
#define WS281X_DEFAULT_PIN = WS281X_PIN
#else
#define WS281X_DEFAULT_PIN
#endif

#ifndef WS281X_CHANNEL
#define WS281X_CHANNEL RMT_CHANNEL_1
#endif

#ifndef WS281X_VARIANT
#define WS281X_VARIANT ws2812
#endif

namespace mesh::peripherals
{
  enum class ws281x_variant
  {
    ws2812,
    ws2815
  };

  class ws281x_strip : public led_strip
  {
  public:
    ws281x_strip(uint8_t pin WS281X_DEFAULT_PIN, ws281x_variant variant = ws281x_variant::WS281X_VARIANT, rmt_channel_t channel = WS281X_CHANNEL);

    virtual ~ws281x_strip() override;

    virtual void push_pixels(std::span<const graphics::color_rgb> pixels) override;
  
  private:
    uint32_t _signal_zero_high_ticks;
    uint32_t _signal_zero_low_ticks;
    uint32_t _signal_one_high_ticks;
    uint32_t _signal_one_low_ticks;
    uint32_t _signal_reset_ticks;
    rmt_channel_t _channel;

    static void convert_data(
      const void* source, 
      rmt_item32_t* destination, 
      size_t source_size, 
      size_t destination_size, 
      size_t* source_advanced_by, 
      size_t* destination_advanced_by);
  };
}