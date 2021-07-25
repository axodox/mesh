#include "json_serialization.hpp"
#include "app/light_strip/helpers/json_serialization.hpp"

using namespace std;
using namespace mesh::app::light_strip::sources;

namespace mesh::infrastructure
{
  template<> const char* name_of<static_source_settings>() { return "static"; }
  template<> const char* name_of<rainbow_source_settings>() { return "rainbow"; }
}

namespace mesh::serialization::json
{
  std::unique_ptr<json_value> json_serializer<static_source_settings>::to_json(const static_source_settings& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("color", value.color);
    return object;
  }

  bool json_serializer<static_source_settings>::from_json(const std::unique_ptr<json_value>& json, static_source_settings& value)
  {
    if (json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json.get());
      object->get_value("color", value.color);
      return true;
    }
    else
    {
      return false;
    }
  }

  std::unique_ptr<json_value> json_serializer<rainbow_source_settings>::to_json(const rainbow_source_settings& value)
  {
    auto object = make_unique<json_object>();
    object->set_value("spatialFrequency", value.spatial_frequency);
    object->set_value("spatialVelocity", value.angular_velocity);
    return object;
  }

  bool json_serializer<rainbow_source_settings>::from_json(const std::unique_ptr<json_value>& json, rainbow_source_settings& value)
  {
    if (json && json->type() == json_type::object)
    {
      auto object = static_cast<const json_object*>(json.get());
      object->get_value("spatialFrequency", value.spatial_frequency);
      object->get_value("spatialVelocity", value.angular_velocity);
      return true;
    }
    else
    {
      return false;
    }
  }
}