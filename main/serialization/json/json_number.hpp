#pragma once
#include <type_traits>
#include <chrono>

#include "json_value.hpp"
#include "infrastructure/traits.hpp"

namespace mesh::serialization::json
{
  struct json_number : public json_value_container<double, json_type::number>
  {
    using json_value_container::json_value_container;
    using json_value::to_string;

    virtual void to_string(std::stringstream& stream) const override;
    static std::unique_ptr<json_number> from_string(std::string_view& text);
  };

  template <typename value_t>
  struct json_serializer<value_t, std::enable_if_t<std::is_arithmetic_v<value_t>, void>>
  {
    static std::unique_ptr<json_value> to_json(value_t value)
    {
      return std::make_unique<json_number>(double(value));
    }

    static bool from_json(const std::unique_ptr<json_value>& json, value_t& value)
    {
      if (json && json->type() == json_type::number)
      {
        value = value_t(static_cast<json_number*>(json.get())->value);
        return true;
      }
      else
      {
        return false;
      }
    }
  };

  template <typename value_t>
  struct json_serializer<value_t, std::enable_if_t<mesh::infrastructure::is_instantiation_of<std::chrono::duration, value_t>::value, void>>
  {
    static std::unique_ptr<json_value> to_json(value_t value)
    {
      return std::make_unique<json_number>(double(value.count()));
    }

    static bool from_json(const std::unique_ptr<json_value>& json, value_t& value)
    {
      if (json && json->type() == json_type::number)
      {
        value = value_t(typename value_t::rep(static_cast<json_number*>(json.get())->value));
        return true;
      }
      else
      {
        return false;
      }
    }
  };

  template <typename value_t>
  struct json_serializer<value_t, std::enable_if_t<std::is_enum_v<value_t>, void>>
  {
    static std::unique_ptr<json_value> to_json(value_t value)
    {
      return std::make_unique<json_number>(double(std::underlying_type_t<value_t>(value)));
    }

    static bool from_json(const std::unique_ptr<json_value>& json, value_t& value)
    {
      if (json && json->type() == json_type::number)
      {
        value = value_t(std::underlying_type_t<value_t>(static_cast<json_number*>(json.get())->value));
        return true;
      }
      else
      {
        return false;
      }
    }
  };
}