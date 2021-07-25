#include "light_source.hpp"
#include "static_source.hpp"
#include "rainbow_source.hpp"
#include "infrastructure/traits.hpp"
#include "serialization/json.hpp"
#define ns app::light_strip::sources

namespace mesh::serialization::json
{
  template<>
  struct json_serializer<ns::static_source_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::static_source_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::static_source_settings& value);
  };

  template<>
  struct json_serializer<ns::rainbow_source_settings>
  {
    static std::unique_ptr<json_value> to_json(const ns::rainbow_source_settings& value);
    static bool from_json(const std::unique_ptr<json_value>& json, ns::rainbow_source_settings& value);
  };

  template <>
  struct json_serializer<std::unique_ptr<ns::light_source_settings>> : public json_multi_serializer<
    ns::light_source_settings,
    ns::static_source_settings,
    ns::rainbow_source_settings>
  { };
}

#undef ns