#pragma once
#include <memory>
#include <string>
#include <sstream>

namespace mesh::serialization::json
{
  enum class json_type
  {
    null,
    boolean,
    number,
    string,
    array,
    object
  };

  template <typename value_t, typename enable_t = void>
  struct json_serializer
  { };

  struct json_value
  {
    inline static const std::unique_ptr<json_value> empty = {};

    virtual ~json_value() = default;

    virtual json_type type() const = 0;
    virtual void to_string(std::stringstream& stream) const = 0;

    std::string to_string() const;

    static std::unique_ptr<json_value> from_string(const char* text);

    static std::unique_ptr<json_value> from_string(std::string_view& text);

    static std::unique_ptr<json_value> from_string(const std::string& text);
  };

  template<typename value_t, json_type json_type_c>
  struct json_value_container : public json_value
  {
    typedef value_t value_type;

    value_t value;

    json_value_container() :
      value(value_t{})
    { }

    json_value_container(const value_t& value) :
      value(value)
    { }

    json_value_container(value_t&& value) :
      value(std::move(value))
    { }

    operator const value_t& () const
    {
      return value;
    }

    virtual json_type type() const override
    {
      return json_type_c;
    }
  };

  void json_skip_whitespace(std::string_view& text);
}