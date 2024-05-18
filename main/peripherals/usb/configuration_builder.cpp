#include "configuration_builder.hpp"
#include "device_builder.hpp"

namespace mesh::peripherals::usb
{
  void configuration_builder::power(uint8_t value)
  {
    configuration()->power = value;
  }

  void configuration_builder::attributes(configuration_attributes value)
  {
    configuration()->attributes = value;
  }

  void configuration_builder::add_hid_interface(bool has_output, const char* name)
  {
    interface_descriptor interface;
    interface.id = configuration()->interface_count;
    interface.endpoint_count = has_output ? 2 : 1;
    interface.type = interface_type::hid;
    _device->set_string(interface.name, name);
    add_description(interface);

    hid_descriptor hid;
    hid.descriptor_count = report_count;
    hid.descriptor_type = descriptor_type::report;
    hid.descriptor_length = report_length;
    add_description(hid);

    endpoint_descriptor endpoint_in;
    endpoint_in.address = endpoint_address(1, endpoint_direction::in);
    endpoint_in.attributes = endpoint_attributes::interrupt;
    endpoint_in.max_packet_size = largest_report_size;
    add_description(endpoint_in);

    if (has_output)
    {
      endpoint_descriptor endpoint_out;
      endpoint_out.address = endpoint_address(2, endpoint_direction::out);
      endpoint_out.attributes = endpoint_attributes::interrupt;
      endpoint_out.max_packet_size = largest_report_size;
      add_description(endpoint_out);
    }
  }

  configuration_builder::configuration_builder(device_builder& device) :
    _device(&device),
    _stream(&device._interfaces)
  {
    add_description(configuration_descriptor{});
    configuration()->id = ++_device->_descriptor.configuration_count;
  }

  configuration_descriptor* configuration_builder::configuration()
  {
    return reinterpret_cast<configuration_descriptor*>(_stream->data());
  }

  void configuration_builder::add_description(const auto& description)
  {
    _stream->write(description);
    configuration()->total_length = uint16_t(_stream->length());
  }

  void configuration_builder::add_interface(const interface_descriptor& interface)
  {
    add_description(interface);
    configuration()->interface_count++;
  }
}