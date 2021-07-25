#pragma once
#include <cstring>
#include <memory>
#include <map>
#include <initializer_list>
#include <typeinfo>
#include <typeindex>
#include "json_value.hpp"
#include "infrastructure/traits.hpp"

namespace mesh::serialization::json
{
  struct type_name_comparer {
    bool operator()(const char* a, const char* b) const {
      return strcmp(a, b) < 0;
    }
  };

  inline const char* type_property_name = "$type";

  template<typename base_t, typename... derived_t>
  class json_multi_serializer
  {
  private:

    //Deserializers
    typedef bool (*from_json_t)(const std::unique_ptr<json_value>& json, std::unique_ptr<base_t>& value, const base_t* base_value);

    struct json_deserializer_t
    {
      from_json_t from_json;
      std::type_index index{typeid(void)};
    };

    typedef std::map<const char*, json_deserializer_t, type_name_comparer> deserializer_map_t;

    template<typename value_t>
    static bool deserialize(const std::unique_ptr<json_value>& json, std::unique_ptr<base_t>& value, const base_t* base_value)
    {
      auto result = std::make_unique<value_t>();

      if(base_value)
      {
        *result = static_cast<const value_t&>(*base_value);
      }

      auto success = json_serializer<value_t>::from_json(json, *result);
      if (success)
      {
        value = std::move(result);
        return true;
      }
      else
      {
        return false;
      }
    }

    template<typename first_t, typename... other_t>
    static void build_deserializer_map(deserializer_map_t& deserializers)
    {
      deserializers[nameof(first_t)] = json_deserializer_t{ &json_multi_serializer::deserialize<first_t>, std::type_index(typeid(first_t)) };

      if constexpr (sizeof...(other_t) > 0)
      {
        build_deserializer_map<other_t...>(deserializers);
      }
    }

    static deserializer_map_t build_deserializer_map()
    {
      deserializer_map_t result;
      if constexpr (sizeof...(derived_t) > 0)
      {
        build_deserializer_map<derived_t...>(result);
      }
      return result;
    }

    inline static deserializer_map_t _deserializers = build_deserializer_map();

    //Serializers
    typedef std::unique_ptr<json_value>(*to_json_t)(const base_t* value);

    struct json_serializer_t
    {
      to_json_t to_json;
      const char* name;
    };

    typedef std::map<std::type_index, json_serializer_t> serializer_map_t;

    template<typename value_t>
    static std::unique_ptr<json_value> serialize(const base_t* value)
    {
      return json_serializer<value_t>::to_json(static_cast<const value_t&>(*value));
    }

    template<typename first_t, typename... other_t>
    static void build_serializer_map(serializer_map_t& serializers)
    {
      serializers[typeid(first_t)] = json_serializer_t{ &json_multi_serializer::serialize<first_t>, nameof(first_t) };

      if constexpr (sizeof...(other_t) > 0)
      {
        build_serializer_map<other_t...>(serializers);
      }
    }

    static serializer_map_t build_serializer_map()
    {
      serializer_map_t result;
      if constexpr (sizeof...(derived_t) > 0)
      {
        build_serializer_map<derived_t...>(result);
      }
      return result;
    }

    inline static serializer_map_t _serializers = build_serializer_map();

  public:
    static std::unique_ptr<json_value> to_json(const base_t* value)
    {
      auto it = _serializers.find(typeid(*value));
      if (it != _serializers.end())
      {
        auto result = it->second.to_json(value);
        if (result->type() == json_type::object)
        {
          auto object = static_cast<json_object*>(result.get());
          object->set_value(type_property_name, it->second.name);
        }
        return result;
      }
      else
      {
        return nullptr;
      }
    }

    static std::unique_ptr<json_value> to_json(const std::unique_ptr<base_t>& value)
    {
      return to_json(value.get());
    }

    static bool from_json(const std::unique_ptr<json_value>& json, std::unique_ptr<base_t>& value, std::initializer_list<const base_t*> base_values = {})
    {
      if (json && json->type() == json_type::object)
      {
        auto object = static_cast<const json_object*>(json.get());
        auto type_name = object->get_value<std::string>(type_property_name);
        auto it = _deserializers.find(type_name.c_str());
        if (it != _deserializers.end())
        {
          for(auto base_value : base_values)
          {
            if(std::type_index(typeid(*base_value)) == it->second.index)
            {
              return it->second.from_json(json, value, base_value);
            }
          }

          return it->second.from_json(json, value, nullptr);
        }
      }

      return false;
    }
  };
}