#pragma once
#include <memory>
#include <string>
#include <functional>
#include <map>
#include <typeinfo>
#include <typeindex>
#include "infrastructure/text.hpp"
#include "infrastructure/traits.hpp"
#include "infrastructure/value_container.hpp"
#include "json_value.hpp"
#include "json_object.hpp"
#include "json_string.hpp"

namespace mesh::serialization::json
{
  class json_container_serializer
  {
    inline static const char* _key_name = "type";
    inline static const char* _value_name = "data";

  private:  
    typedef std::function<infrastructure::value_container(const std::unique_ptr<json_value>&)> constructor_t;
    std::map<const char*, constructor_t, infrastructure::cstring_comparer> _constructors;
    std::map<std::type_index, const char*> _type_names;

  public:
    template<typename value_t>
    void add_multi_type(const char* name, std::initializer_list<const value_t*> base_values = {})
    {
      _constructors.emplace(name, [=](const std::unique_ptr<json_value>& json) {
        std::unique_ptr<value_t> value;
        if(json_serializer<std::unique_ptr<value_t>>::from_json(json, value, base_values))
        {
          return infrastructure::value_container::create(std::move(value));
        }
        else
        {
          return infrastructure::value_container{};
        }
        });
      _type_names.emplace(std::type_index(typeid(value_t)), name);
    }

    template<typename value_t, typename = std::enable_if_t<std::is_copy_assignable_v<value_t>>>
    void add_type(const char* name, const value_t* base_value = nullptr)
    {
      _constructors.emplace(name, [=](const std::unique_ptr<json_value>& json) { 
        value_t value{};
        if(base_value)
        {
          value = *base_value;
        }

        if(json_serializer<value_t>::from_json(json, value))
        {
          return infrastructure::value_container::create(std::move(value));
        }
        else
        {
          return infrastructure::value_container{};
        }
        });
      _type_names.emplace(std::type_index(typeid(value_t)), name);
    }

    infrastructure::value_container from_json(const std::unique_ptr<json_value>& json, const char*& type);

    template<typename value_t>
    std::unique_ptr<json_value> to_json(const value_t& value)
    {
      auto it = _type_names.find(std::type_index(typeid(value_t)));
      if(it != _type_names.end())
      {
        auto result = std::make_unique<json_object>();
        (*result)[_key_name] = std::make_unique<json_string>(it->second);
        (*result)[_value_name] = json_serializer<value_t>::to_json(value);
        return result;
      }
      else
      {
        return nullptr;
      }
    }
  };
}