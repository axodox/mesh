#include <chrono>
#include <thread>

#include "uart_source.hpp"
#include "infrastructure/bitwise_operations.hpp"
#include "infrastructure/dependencies.hpp"
#include "infrastructure/error.hpp"
#include "peripherals/uart_stream.hpp"
#include "peripherals/integrated_led.hpp"
#include "storage/serializer.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::infrastructure;
using namespace mesh::graphics;
using namespace mesh::threading;
using namespace mesh::peripherals;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::sources
{
  uart_source::uart_source(light_strip_context &context) : 
    light_source(context),
    _thread([=] { receive_data(); }, task_affinity::core_1, task_priority::normal, "uart_source")
  {
    _properties.steady_frame_source = false;
    _properties.is_passthrough = true;
  }

  light_source_type uart_source::type() const
  {
    return light_source_type::uart_source;
  }

  const light_source_settings *uart_source::get_settings() const
  {
    return &_context.settings.uart_source;
  }

  void uart_source::fill(infrastructure::array_view<graphics::color_rgb> &pixels)
  {
    lock_guard lock(_mutex);
    auto copied_length = min(pixels.size(), _buffer.size()) * sizeof(color_rgb);
    auto zeroed_length = pixels.size() * sizeof(color_rgb) - copied_length;

    if (copied_length > 0) memcpy((uint8_t *)pixels.data(), _buffer.data(), copied_length);
    if (zeroed_length > 0) memset((uint8_t *)pixels.data() + copied_length, 0, zeroed_length);
  }

  void uart_source::receive_data()
  {
    auto led = dependencies.resolve<integrated_led>();
    vector<color_rgb> front_buffer, back_buffer;

    led->state(false);

    uart_stream stream;
    while (_thread.is_running())
    {
      //Initialize UART
      if (stream.baud_rate() != _context.settings.uart_source.baud_rate)
      {
        _logger.log_message(log_severity::info, "Initializing UART...");
        stream = uart_stream(UART_NUM_0, _context.settings.uart_source.baud_rate);
        _logger.log_message(log_severity::info, "UART ready.");
      }

      //Read header
      led->state(false);
      auto led_count = read_header(stream);
      led->state(led_count > 0);
      
      //Try again if later if failed
      if(led_count == 0) continue;

      //Resize buffer as needed
      if (led_count != back_buffer.size())
      {
        back_buffer.resize(led_count);
      }

      //Read colors
      array_view buffer_view{ back_buffer };
      stream.read(buffer_view);

      //Validate checksum
      uint32_t checksum = 0;
      for(auto& color : back_buffer)
      {
        uint32_t hash = (uint32_t(color.r) << 24) + (uint32_t(color.g) << 16) + uint32_t(color.b);
        checksum = (checksum + 0x9e3779b9 + (hash << 6) + (hash >> 2)) ^ hash;
      }

      //If checksum is valid switch buffers
      if(stream.read<uint32_t>() == checksum)
      {
        swap(back_buffer, front_buffer);

        {
          lock_guard lock(_mutex);
          _buffer = front_buffer;
        }

        _context.frame_ready.set();
        stream.flush();
      }
    }

    led->state(false);
  }

  uint32_t uart_source::read_header(storage::stream &stream)
  {
    if (stream.read<uint8_t>() != 0x41) return 0u;
    if (stream.read<uint8_t>() != 0x64) return 0u;
    if (stream.read<uint8_t>() != 0x61) return 0u;

    auto high_count = stream.read<uint8_t>();
    auto low_count = stream.read<uint8_t>();
    auto checksum_count = uint8_t(high_count ^ low_count ^ 0x55);
    if (stream.read<uint8_t>() != checksum_count) return 0u;

    return (high_count << 8) + low_count + 1;
  }
}