#include "ws281x_strip.hpp"
#include "driver/rmt.h"
#include "infrastructure/error.hpp"

using namespace std;
using namespace mesh::infrastructure;
using namespace mesh::graphics;

namespace mesh::peripherals
{
  ws281x_strip::ws281x_strip(uint8_t pin, ws281x_variant variant, rmt_channel_t channel) :
    _channel(channel)
  {
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(gpio_num_t(pin), _channel);
    config.clk_div = 1;    
    check_result(rmt_config(&config));
    check_result(rmt_driver_install(_channel, 0, 0));

    uint32_t clock_frequency_hz;
    check_result(rmt_get_counter_clock(_channel, &clock_frequency_hz));

    float clock_frequency_ghz = float(clock_frequency_hz) / 1e9;
    switch(variant)
    {
    case ws281x_variant::ws2812:
      _signal_zero_high_ticks = uint32_t(clock_frequency_ghz * 400); //.4us
      _signal_zero_low_ticks = uint32_t(clock_frequency_ghz * 850); //.85us
      _signal_one_high_ticks = uint32_t(clock_frequency_ghz * 800); //.8us
      _signal_one_low_ticks = uint32_t(clock_frequency_ghz * 450); //.45us
      _signal_reset_ticks = uint32_t(clock_frequency_ghz * 50000);
      break;
    case ws281x_variant::ws2815:
      _signal_zero_high_ticks = uint32_t(clock_frequency_ghz * 250); //.3us
      _signal_zero_low_ticks = uint32_t(clock_frequency_ghz * 1100); //1.09us
      _signal_one_high_ticks = uint32_t(clock_frequency_ghz * 1100); //1.09us
      _signal_one_low_ticks = uint32_t(clock_frequency_ghz * 250); //.32us
      _signal_reset_ticks = uint32_t(clock_frequency_ghz * 350000);
      break;
    }    

    check_result(rmt_translator_init(_channel, &ws281x_strip::convert_data));
    check_result(rmt_translator_set_context(_channel, this));
  }

  ws281x_strip::~ws281x_strip()
  {
    check_result(rmt_driver_uninstall(_channel));
  }

  void ws281x_strip::push_pixels(std::span<const graphics::color_rgb> pixels)
  {
    check_result(rmt_write_sample(_channel, reinterpret_cast<const uint8_t*>(pixels.data()), pixels.size_bytes(), true));
    
    const rmt_item32_t resetbit = {{{ 0, 1, _signal_reset_ticks, 0 }}};
    rmt_write_items(_channel, &resetbit, 1, true);
  }

  void ws281x_strip::convert_data(
    const void* source_start, 
    rmt_item32_t* destination_start, 
    size_t source_size, 
    size_t destination_size, 
    size_t* source_advanced_by, 
    size_t* destination_advanced_by)
  {
    ws281x_strip* that;
    check_result(rmt_translator_get_context(destination_advanced_by, reinterpret_cast<void**>(&that)));

    if(!source_start || !destination_start)
    {
      *source_advanced_by = 0;
      *destination_advanced_by = 0;
      return;
    }

    const rmt_item32_t bit0 = {{{ that->_signal_zero_high_ticks, 1, that->_signal_zero_low_ticks, 0 }}};
    const rmt_item32_t bit1 = {{{ that->_signal_one_high_ticks, 1, that->_signal_one_low_ticks, 0 }}};
    auto source = reinterpret_cast<const uint8_t*>(source_start);
    auto destination = reinterpret_cast<rmt_item32_t*>(destination_start);
    
    auto count = min(source_size, destination_size / 8);
    for(size_t byte = 0; byte < count; byte++)
    {
      for(auto bit = 0; bit < 8; bit++)
      {
        auto is_one = *source & (1 << (7 - bit));
        if(is_one)
        {
          *destination++ = bit1;
        }
        else
        {
          *destination++ = bit0;
        }
      }
      source++;
    }
    *source_advanced_by = count;
    *destination_advanced_by = count * 8;
  }
}