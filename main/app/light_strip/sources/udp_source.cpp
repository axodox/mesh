#include "udp_source.hpp"
#include "infrastructure/bitwise_operations.hpp"
#include "lwip/sockets.h"

#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace mesh::infrastructure;
using namespace mesh::graphics;
using namespace mesh::threading;
using namespace mesh::app::light_strip::settings;

enum class light_data_flags : uint16_t
{
  none = 0,
  passthrough = 1 << 0
};

struct light_data_header
{
  static const uint32_t magic = 0x21C64BE5;

  uint32_t protocol_id;
  uint32_t message_id;
  light_data_flags flags;
  uint16_t light_count;
};

namespace mesh::app::light_strip::sources
{
  udp_source::udp_source(light_strip_context& context) :
    light_source(context)
  {
    _properties.steady_frame_source = false;
    _thread = make_unique<task>([=] { receive_data(); }, task_affinity::core_1, task_priority::normal);
  }

  light_source_type udp_source::type() const
  {
    return light_source_type::udp_source;
  }

  const light_source_settings* udp_source::get_settings() const
  {
    return &_context.settings.udp_source;
  }

  void udp_source::fill(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    lock_guard<mutex> lock(_mutex);
    auto copied_length = min(pixels.size(), _buffer.size()) * sizeof(color_rgb);
    auto zeroed_length = pixels.size() * sizeof(color_rgb) - copied_length;

    if(copied_length > 0) memcpy((uint8_t*)pixels.data(), _buffer.data(), copied_length);
    if(zeroed_length > 0) memset((uint8_t*)pixels.data() + copied_length, 0, zeroed_length);
  }

  void udp_source::receive_data()
  {
    uint16_t port = ~0;
    int socket_handle = 0;
    steady_clock::time_point last_refresh{};
    uint32_t message_id = 0;
    std::vector<uint8_t> buffer;

    while(true)
    {
      //Initialize socket
      if(port != _context.settings.udp_source.port)
      {
        port = _context.settings.udp_source.port;
        if(socket_handle) closesocket(socket_handle);

        sockaddr_in address{};
        address.sin_len = sizeof(sockaddr_in);
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = IPADDR_ANY;
        address.sin_port = htons(port);
        socket_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

        timeval timeout {0, 100000};
        setsockopt(socket_handle, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        bind(socket_handle, (sockaddr*)&address, sizeof(address));

        _logger.log_message(log_severity::info, "Listening at: %d", port);
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
      auto true_length = recv(socket_handle, buffer.data(), buffer.size() - 1, 0);

      //Check header length
      if(true_length < sizeof(light_data_header)) 
      {
        this_thread::sleep_for(milliseconds(8));
        continue;
      }

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