#pragma once
#include "descriptors.hpp"
#include "storage/memory_stream.hpp"

namespace mesh::peripherals::usb
{
  class device_builder;

  class configuration_builder
  {
    friend class device_builder;

  public:
    void power(uint8_t value);
    void attributes(configuration_attributes value);

    void add_hid_interface(bool hasOutput, const char* name = nullptr);

  private:
    device_builder* _device;
    storage::memory_stream* _stream;

    configuration_builder(device_builder& device);
    configuration_descriptor* configuration();
    void add_description(const auto& description);
    void add_interface(const interface_descriptor& interface);
  };
}