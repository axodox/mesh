#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <map>
#include <functional>

namespace mesh::json
{
  template<template<typename...> class, typename...>
  struct is_instantiation_of : public std::false_type {};

  template<template<typename...> class U, typename... T>
  struct is_instantiation_of<U, U<T...>> : public std::true_type {};

  enum class json_type
  {
    null,
    boolean,
    number,
    string,
    array,
    object,
    serializable
  };

  struct json_value
  {
    inline static const std::unique_ptr<json_value> empty = {};

    virtual ~json_value() = default;

    virtual json_type type() const = 0;
    virtual std::string to_string() const = 0;

    static std::unique_ptr<json_value> from_string(const char* text);

    static std::unique_ptr<json_value> from_string(std::string_view& text);

    static std::unique_ptr<json_value> from_string(const std::string& text);

    template<typename value_t>
    static std::unique_ptr<json_value> from_value(const value_t& value);

    template<typename value_t>
    static std::unique_ptr<json_value> move_value(value_t&& value);

    template<typename value_t>
    static bool to_value(const json_value* source, value_t& target);
  };

  template<typename value_t, json_type json_type_c>
  struct json_value_base : public json_value
  {
    typedef value_t value_type;

    value_t value;

    json_value_base() :
      value(value_t{})
    { }

    json_value_base(const value_t& value) :
      value(value)
    { }

    json_value_base(value_t&& value) :
      value(std::move(value))
    { }

    operator const value_t&() const
    {
      return value;
    }

    virtual json_type type() const override
    {
      return json_type_c;
    }
  };

  struct json_null : public json_value
  {
    virtual json_type type() const override;

    virtual std::string to_string() const override;
    static std::unique_ptr<json_null> from_string(std::string_view& text);
  };

  struct json_boolean : public json_value_base<bool, json_type::boolean>
  {
    using json_value_base::json_value_base;

    virtual std::string to_string() const override;
    static std::unique_ptr<json_boolean> from_string(std::string_view& text);
  };

  struct json_number : public json_value_base<double, json_type::number>
  {
    using json_value_base::json_value_base;

    virtual std::string to_string() const override;
    static std::unique_ptr<json_number> from_string(std::string_view& text);
  };

  struct json_string : public json_value_base<std::string, json_type::string>
  {
    using json_value_base::json_value_base;

    virtual std::string to_string() const override;
    static std::unique_ptr<json_string> from_string(std::string_view& text);
  };

  struct json_array : public json_value_base<std::vector<std::unique_ptr<json_value>>, json_type::array>
  {
    using json_value_base::json_value_base;

    std::unique_ptr<json_value>& operator[](size_t index);
    const std::unique_ptr<json_value>& operator[](size_t index) const;

    virtual std::string to_string() const override;
    static std::unique_ptr<json_array> from_string(std::string_view& text);
  };

  struct json_object : public json_value_base<std::unordered_map<std::string, std::unique_ptr<json_value>>, json_type::object>
  {
    using json_value_base::json_value_base;

    std::unique_ptr<json_value>& operator[](const std::string& property);
    const std::unique_ptr<json_value>& operator[](const std::string& property) const;

    std::unique_ptr<json_value>& at(const std::string& property);
    const std::unique_ptr<json_value>& at(const std::string& property) const;

    template<typename value_t>
    void set_value(const std::string& key, const value_t& value)
    {
      this->value[key] = json_value::from_value(value);
    }

    template<typename value_t>
    void set_value(const std::string& key, value_t&& value)
    {
      this->value[key] = json_value::move_value(std::move(value));
    }

    template<typename value_t>
    bool get_value(const std::string& key, value_t& value) const
    {
      auto it = this->value.find(key);
      if (it != this->value.end())
      {
        auto container = it->second.get();

        return json_value::to_value(container, value);
      }

      return false;
    }

    template<typename value_t>
    value_t get_value(const std::string& key) const
    {
      value_t value{};
      get_value(key, value);
      return value;
    }

    virtual std::string to_string() const override;
    static std::unique_ptr<json_object> from_string(std::string_view& text);
  };

  struct json_serializable_base : public json_value
  {
    virtual std::unique_ptr<json_value> to_json() const = 0;
    virtual void from_json(const json_value* value) = 0;

    virtual json_type type() const
    {
      return json_type::serializable;
    }

    virtual std::string to_string() const override
    {
      auto json = to_json();
      if (json)
      {
        return json->to_string();
      }
      else
      {
        return "null";
      }
    }

    void load_string(std::string_view& text)
    {
      auto value = json_value::from_string(text);

      if (value)
      {
        from_json(value.get());
      }
    }
  };

  template<typename base_t, typename... derived_t>
  class json_serializable : public json_serializable_base
  {
  private:
    template<typename first_t, typename... other_t>
    static void build_constructor_map(std::map<std::string, std::function<std::unique_ptr<base_t>()>>& constructors)
    {
      if constexpr (sizeof...(other_t) == 0)
      {
        constructors[std::make_unique<first_t>()->type_name()] = []() -> std::unique_ptr<base_t> { return std::make_unique<first_t>(); };
      }
      else
      {
        build_constructor_map<other_t...>(constructors);
      }
    }

    static std::map<std::string, std::function<std::unique_ptr<base_t>()>> build_constructor_map()
    {
      std::map<std::string, std::function<std::unique_ptr<base_t>()>> result;
      if constexpr (sizeof...(derived_t) > 0)
      {
        build_constructor_map<derived_t...>(result);
      }
      return result;
    }
    
    inline static std::map<std::string, std::function<std::unique_ptr<base_t>()>> _constructors = build_constructor_map();

  protected:
    void add_type_info(json_object* object) const
    {
      object->set_value("$type", type_name());
    }

  public:
    virtual std::string type_name() const
    {
      return "default";
    }

    static std::unique_ptr<base_t> from_string(std::string_view& text)
    {
      if constexpr (sizeof...(derived_t) == 0)
      {
        auto result = std::make_unique<base_t>();
        result->load_string(text);
        return result;
      }
      else
      {
        auto json = json_object::from_string(text);
        if (!json) return nullptr;
        auto type = json->get_value<std::string>("$type");
        if (type.empty()) return nullptr;

        auto it = _constructors.find(type);
        if (it == _constructors.end()) return nullptr;
        auto result = it->second();
        result->from_json(json.get());
        return result;
      }
    }

    static std::unique_ptr<base_t> from_string(const std::string& text)
    {
      std::string_view view = text;
      return from_string(view);
    }
  };

  template<typename value_t>
  std::unique_ptr<json_value> json_value::from_value(const value_t& value)
  {
    if constexpr (std::is_same_v<nullptr_t, value_t>)
    {
      return std::make_unique<json_null>();
    }
    else if constexpr (std::is_same_v<bool, value_t>)
    {
      return std::make_unique<json_boolean>(value);
    }
    else if constexpr (std::is_arithmetic_v<value_t>)
    {
      return std::make_unique<json_number>(double(value));
    }
    else if constexpr (std::is_convertible_v<value_t, std::string>)
    {
      return std::make_unique<json_string>(std::string(value));
    }
    else if constexpr (std::is_same_v<json_array::value_type, value_t>)
    {
      return std::make_unique<json_array>(value);
    }
    else if constexpr (is_instantiation_of<std::vector, value_t>::value)
    {
      auto result = std::make_unique<json_array>();
      for (auto it = value.begin(); it != value.end(); it++)
      {
        result->value.push_back(json_value::from_value<value_t::value_type>(*it));
      }
      return result;
    }
    else if constexpr (std::is_same_v<json_object::value_type, value_t>)
    {
      return std::make_unique<json_object>(value);
    }
    else
    {
      return nullptr;
      //static_assert(false, "Failed to convert type to JSON"); -> triggered by GCC for some reason
    }
  }

  template<typename value_t>
  std::unique_ptr<json_value> json_value::move_value(value_t&& value)
  {
    if constexpr (is_instantiation_of<std::unique_ptr, value_t>::value)
    {
      static_assert(std::is_base_of_v<json_value, typename value_t::element_type>, "Pointed value must be of json_value for passthrough!");
      return std::move(value);
    }
    else if constexpr (std::is_base_of_v<json_value, std::remove_reference_t<value_t>>)
    {
      return std::make_unique<std::remove_reference_t<value_t>>(std::move(value));
    }
    else if constexpr (std::is_same_v<json_string::value_type, value_t>)
    {
      return std::make_unique<json_string>(std::move(value));
    }
    else if constexpr (std::is_same_v<json_array::value_type, value_t>)
    {
      return std::make_unique<json_array>(std::move(value));
    }
    else if constexpr (is_instantiation_of<std::vector, value_t>::value)
    {
      auto result = std::make_unique<json_array>();
      for (auto it = value.begin(); it != value.end(); it++)
      {
        result->value.push_back(json_value::move_value<value_t::value_type>(std::move(*it)));
      }
      return result;
    }
    else if constexpr (std::is_same_v<json_object::value_type, value_t>)
    {
      return std::make_unique<json_object>(std::move(value));
    }
    else
    {
      return json_value::from_value((const value_t&)value);
    }
  }

  template<typename value_t>
  bool json_value::to_value(const json_value* source, value_t& target)
  {
    switch (source->type())
    {
    case json_type::boolean:
      if constexpr (std::is_same_v<bool, value_t>)
      {
        target = static_cast<const json_boolean*>(source)->value;
        return true;
      }
      break;
    case json_type::number:
      if constexpr (std::is_arithmetic_v<value_t>)
      {
        target = value_t(static_cast<const json_number*>(source)->value);
        return true;
      }
      break;
    case json_type::string:
      if constexpr (std::is_assignable_v<value_t, std::string>)
      {
        target = value_t(static_cast<const json_string*>(source)->value);
        return true;
      }
      break;
    case json_type::serializable:
      if constexpr (std::is_base_of_v<json_serializable_base, value_t>)
      {
        if (source->type() == json_type::serializable)
        {
          target = reinterpret_cast<const value_t&>(*source);
        }
        else
        {
          target.from_json(source);
        }
        return true;
      }
      break;
    default:
      return false;
    }

    return false;
  }
}