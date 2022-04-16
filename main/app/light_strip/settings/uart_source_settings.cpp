#include "uart_source_settings.hpp"
#include "app/light_strip/helpers/json_serialization.hpp"

using namespace std;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::settings
{
  light_source_type uart_source_settings::type() const
  {
    return light_source_type::uart_source;
  }
}

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<uart_source_settings>::to_json(const uart_source_settings& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("baudRate", value.baud_rate);
    return object;
  }

  bool json_serializer<uart_source_settings>::from_json(const std::unique_ptr<json_value>& json, uart_source_settings& value)
  {
    if (json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json.get());
      object->get_value("baudRate", value.baud_rate);
      return true;
    }
    else
    {
      return false;
    }
  }
}