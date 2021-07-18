#pragma once
#include "led_strip.hpp"
#include "driver/rmt.h"
#include <chrono>

namespace mesh::peripherals
{
  class ws2812_strip : public led_strip
  {
  public:
    ws2812_strip(uint8_t pin = 18, rmt_channel_t channel = RMT_CHANNEL_1);
    virtual ~ws2812_strip() override;

    virtual void push_pixels(const infrastructure::array_view<graphics::color_rgb>& pixels) override;
  
  private:
    uint32_t _signal_zero_high_ticks;
    uint32_t _signal_zero_low_ticks;
    uint32_t _signal_one_high_ticks;
    uint32_t _signal_one_low_ticks;
    uint32_t _signal_reset_low_ticks;
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