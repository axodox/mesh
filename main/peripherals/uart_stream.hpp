#pragma once
#include "storage/stream.hpp"
#include "driver/uart.h"

namespace mesh::peripherals
{
  class uart_stream : public storage::stream
  {
  public:
    uart_stream();
    uart_stream(uart_port_t port, uint32_t baud_rate);
    ~uart_stream();

    uart_stream(uart_stream&&);
    uart_stream& operator =(uart_stream&&);

    uart_stream(const uart_stream&) = delete;
    uart_stream& operator =(const uart_stream&) = delete;
    
    using stream::read;
    using stream::write;

    uint32_t baud_rate() const;
    uart_port_t port() const;
    operator bool() const;
    
    virtual void write(std::span<const uint8_t> buffer) override;
    virtual void read(std::span<uint8_t> buffer) override;
    virtual void flush() override;

  private:
    static const uart_port_t _invalid_port = uart_port_t(~0);
    uart_port_t _port;
    uint32_t _baud_rate;
  };
}