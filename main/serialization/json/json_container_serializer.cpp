#include "json_container_serializer.hpp"

using namespace std;

namespace mesh::serialization::json
{
  infrastructure::value_container json_container_serializer::from_json(const std::unique_ptr<json_value>& json, const char*& type)
  {
    if(!json || json->type() != json_type::object) return {};

    auto& json_object = *static_cast<const json::json_object*>(json.get());
    auto& json_type = json_object[_key_name];
    auto& json_data = json_object[_value_name];
    if(json_type == json_value::empty || json_type->type() != json_type::string || json_data == json_value::empty) return {};

    type = static_cast<const json_string*>(json_type.get())->value.c_str();
    auto it = _constructors.find(type);
    if(it != _constructors.end())
    {
      type = it->first;
      return it->second(json_data);
    }
    else
    {
      type = nullptr;
      return {};
    }
  }
}