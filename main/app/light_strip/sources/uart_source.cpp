#include "uart_source.hpp"
#include "infrastructure/bitwise_operations.hpp"
#include "infrastructure/error.hpp"

#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace mesh::infrastructure;
using namespace mesh::graphics;
using namespace mesh::threading;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::sources
{
  uart_source::uart_source(light_strip_context& context) :
    light_source(context),
    _thread([=] { receive_data(); }, task_affinity::core_1, task_priority::normal, "uart_source")
  {
    _properties.steady_frame_source = false;
  }

  light_source_type uart_source::type() const
  {
    return light_source_type::uart_source;
  }

  const light_source_settings* uart_source::get_settings() const
  {
    return &_context.settings.uart_source;
  }

  void uart_source::fill(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    lock_guard<mutex> lock(_mutex);
    auto copied_length = min(pixels.size(), _buffer.size()) * sizeof(color_rgb);
    auto zeroed_length = pixels.size() * sizeof(color_rgb) - copied_length;

    if(copied_length > 0) memcpy((uint8_t*)pixels.data(), _buffer.data(), copied_length);
    if(zeroed_length > 0) memset((uint8_t*)pixels.data() + copied_length, 0, zeroed_length);
  }

  void uart_source::receive_data()
  {
    uint32_t baud_rate = ~0;
    steady_clock::time_point last_refresh{};
    vector<uint8_t> buffer;

    while(_thread.is_running())
    {
      //Initialize socket
      if(baud_rate != _context.settings.uart_source.baud_rate)
      {
        baud_rate = _context.settings.uart_source.baud_rate;

        throw_result(uart_driver_install(UART_NUM_0, 1024, 1024, 0, nullptr, 0));
        uart_config_t uart_config = {
          .baud_rate = baud_rate,
          .data_bits = UART_DATA_8_BITS,
          .parity = UART_PARITY_DISABLE,
          .stop_bits = UART_STOP_BITS_1,
          .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
          .rx_flow_ctrl_thresh = 122,
        };
        throw_result(uart_param_config(UART_NUM_0, &uart_config));
        
        _logger.log_message(log_severity::info, "Baud rate set to: %d", baud_rate);
      }

      //Prepare buffers
      auto buffer_length = sizeof(light_data_header) + _context.settings.device.light_count * sizeof(color_rgb);
      if(buffer.size() != buffer_length)
      {
        lock_guard<mutex> lock(_mutex);
        buffer.resize(buffer_length);
        _buffer = {
          reinterpret_cast<color_rgb*>(buffer.data() + sizeof(light_data_header)),
          reinterpret_cast<color_rgb*>(buffer.data() + buffer.size())
        };
      }

      //Receive message
      auto true_length = recv(udp_socket.get(), buffer.data(), buffer.size() - 1, 0);

      //Check header length
      if(true_length < 0 || true_length < sizeof(light_data_header)) continue;

      //Check protocol id
      auto& header = *reinterpret_cast<const light_data_header*>(buffer.data());
      if(header.protocol_id != light_data_header::magic) continue;

      //Check message id
      auto now = steady_clock::now();
      if(now - last_refresh < seconds(5) && message_id >= header.message_id) continue;

      last_refresh = now;
      message_id = header.message_id;

      //Read properties
      _properties.is_passthrough = has_flag(header.flags, light_data_flags::passthrough);

      //Read lights
      _buffer = {
        reinterpret_cast<color_rgb*>(buffer.data() + sizeof(light_data_header)),
        reinterpret_cast<color_rgb*>(buffer.data() + min(buffer.size(), sizeof(light_data_header) + sizeof(color_rgb) * header.light_count))
      };
      _context.frame_ready.set();

      this_thread::yield();
    }
  }
}