#pragma once
#include "descriptors.hpp"
#include "storage/memory_stream.hpp"
#include "configuration_builder.hpp"

namespace mesh::peripherals::usb
{
  class device_builder
  {
    friend class configuration_builder;

  public:
    device_builder();

    void vendor(uint16_t id, const char* value);
    void product(uint16_t id, const char* value);
    void serial(const char* value);

    configuration_builder add_configuration();

    void set_string(uint8_t& id, const char* value);

  private:
    device_descriptor _descriptor{};
    std::vector<const char*> _strings;
    storage::memory_stream _interfaces;
  };
}