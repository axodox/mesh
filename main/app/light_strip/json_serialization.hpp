#include "light_strip_controller.hpp"
#include "serialization/json.hpp"

#define ns app::light_strip

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::light_strip_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::light_strip_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::light_strip_settings& value);
  };
}

#undef ns