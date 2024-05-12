#include "uart_stream.hpp"
#include "infrastructure/error.hpp"
#include "threading/task.hpp"

using namespace std;
using namespace mesh::infrastructure;
using namespace mesh::threading;

namespace mesh::peripherals
{
  uart_stream::uart_stream() :
    _port(~0),
    _baud_rate(0)
  { }

  uart_stream::uart_stream(uart_port_t port, uint32_t baud_rate) :
    _port(port),
    _baud_rate(baud_rate)
  {
    if(uart_is_driver_installed(_port))
    {
      check_result(uart_driver_delete(_port));
    }

    check_result(uart_driver_install(_port, 2048, 0, 0, nullptr, ESP_INTR_FLAG_IRAM));

    uart_config_t uart_config = {
      .baud_rate = int(_baud_rate),
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .rx_flow_ctrl_thresh = 122,
      .source_clk = UART_SCLK_APB
    };
    check_result(uart_param_config(_port, &uart_config));
  }

  uart_stream::~uart_stream()
  {
    if (_port != ~0)
    {
      check_result(uart_driver_delete(_port));
      _port = ~0;
      _baud_rate = 0;
    }
  }

  uart_stream::uart_stream(uart_stream &&other)
  {
    _port = other._port;
    _baud_rate = other._baud_rate;
    other._port = ~0;
    other._baud_rate = 0;
  }

  uart_stream &uart_stream::operator=(uart_stream &&other)
  {
    _port = other._port;
    _baud_rate = other._baud_rate;
    other._port = ~0;
    other._baud_rate = 0;
    return *this;
  }

  uint32_t uart_stream::baud_rate() const
  {
    return _baud_rate;
  }

  uart_port_t uart_stream::port() const
  {
    return _port;
  }

  uart_stream::operator bool() const
  {
    return _port != ~0;
  }

  void uart_stream::write(size_t length, const uint8_t *bytes)
  {
    size_t sent_bytes = 0;
    while (sent_bytes < length)
    {
      auto result = uart_write_bytes(_port, bytes, length);
      if(result < 0) check_result(result);

      sent_bytes += result;
      bytes += result;
    }
  }

  void uart_stream::read(size_t length, uint8_t *bytes)
  {
    auto current_task = task::current();

    size_t received_bytes = 0;
    while (received_bytes < length)
    {
      if(current_task && !current_task->is_running())
      {
        memset(bytes, 0, length);
        break;
      }
      else
      {
        auto result = uart_read_bytes(_port, bytes, length, 50);
        if(result < 0) check_result(result);

        received_bytes += result;
        bytes += result;
      }
    }
  }

  void uart_stream::flush()
  {
    check_result(uart_flush(_port));
  }
}