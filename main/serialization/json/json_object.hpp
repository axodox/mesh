#pragma once
#include <unordered_map>
#include "json_value.hpp"

namespace mesh::serialization::json
{
  struct json_object : public json_value_container<std::unordered_map<std::string, std::unique_ptr<json_value>>, json_type::object>
  {
    using json_value_container::json_value_container;
    using json_value::to_string;

    std::unique_ptr<json_value>& operator[](const char* property);
    const std::unique_ptr<json_value>& operator[](const char* property) const;

    std::unique_ptr<json_value>& at(const char* property);
    const std::unique_ptr<json_value>& at(const char* property) const;

    virtual void to_string(std::stringstream& stream) const override;
    static std::unique_ptr<json_object> from_string(std::string_view& text);

    template<typename value_t>
    void set_value(const char* key, const value_t& value)
    {
      this->value[key] = json_serializer<value_t>::to_json(value);
    }

    template<typename value_t>
    void get_value(const char* key, value_t& value) const
    {
      json_serializer<value_t>::from_json(at(key), value);
    }

    template<typename value_t>
    value_t get_value(const char* key) const
    {
      value_t value;
      get_value(key, value);
      return value;
    }
  };
}