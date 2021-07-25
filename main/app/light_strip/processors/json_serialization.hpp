#include "brightness_processor.hpp"
#include "serialization/json.hpp"

#define ns app::light_strip::processors

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::brightness_processor_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::brightness_processor_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::brightness_processor_settings& value);
  };
}

#undef ns