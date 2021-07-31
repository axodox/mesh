#include "udp_source_settings.hpp"
#include "app/light_strip/helpers/json_serialization.hpp"

using namespace std;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip::settings
{
  light_source_type udp_source_settings::type() const
  {
    return light_source_type::udp_source;
  }
}

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<udp_source_settings>::to_json(const udp_source_settings& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("port", value.port);
    return object;
  }

  bool json_serializer<udp_source_settings>::from_json(const std::unique_ptr<json_value>& json, udp_source_settings& value)
  {
    if (json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json.get());
      object->get_value("port", value.port);
      printf("port: %d\n", value.port);
      return true;
    }
    else
    {
      return false;
    }
  }
}