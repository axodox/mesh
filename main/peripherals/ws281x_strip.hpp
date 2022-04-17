#pragma once
#include <chrono>
#include <vector>

#include "led_strip.hpp"
#include "driver/rmt.h"

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
    ws281x_strip(ws281x_variant variant = ws281x_variant::ws2815, uint8_t pin = 18, rmt_channel_t channel = RMT_CHANNEL_1);
    virtual ~ws281x_strip() override;

    virtual void push_pixels(const infrastructure::array_view<graphics::color_rgb>& pixels) override;
  
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