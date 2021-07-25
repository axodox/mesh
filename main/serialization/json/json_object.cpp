#include "json_object.hpp"
#include "json_string.hpp"
#include <sstream>

using namespace std;

namespace mesh::serialization::json
{
  std::unique_ptr<json_value>& json_object::operator[](const char* property)
  {
    return at(property);
  }

  const std::unique_ptr<json_value>& json_object::operator[](const char* property) const
  {
    return at(property);
  }

  std::unique_ptr<json_value>& json_object::at(const char* property)
  {
    return value[property];
  }

  const std::unique_ptr<json_value>& json_object::at(const char* property) const
  {
    auto it = value.find(property);
    if (it != value.end())
    {
      return it->second;
    }
    else
    {
      return json_value::empty;
    }
  }

  void json_object::to_string(std::stringstream& stream) const
  {
    bool is_first = true;
    stream << "{";
    for (auto& [key, value] : value)
    {
      if (is_first)
      {
        is_first = false;
      }
      else
      {
        stream << ",";
      }

      stream << "\"" << key << "\":";
      if (value)
      {
        value->to_string(stream);
      }
      else
      {
        stream << "null";
      }
    }
    stream << "}";
  }

  std::unique_ptr<json_object> json_object::from_string(std::string_view& text)
  {
    if (text.empty() || text[0] != '{') return nullptr;

    unordered_map<string, unique_ptr<json_value>> items;
    text = text.substr(1);
    while (true)
    {
      json_skip_whitespace(text);
      if (text.empty()) return nullptr;
      if (text[0] == '}')
      {
        text = text.substr(1);
        return make_unique<json_object>(move(items));
      }

      auto key = json_string::from_string(text);
      if (!key) return nullptr;

      json_skip_whitespace(text);
      if (text.empty() || text[0] != ':') return nullptr;
      text = text.substr(1);

      auto value = json_value::from_string(text);
      if (!value) return nullptr;

      items[key->value] = move(value);

      json_skip_whitespace(text);
      if (!text.empty() && text[0] == ',')
      {
        text = text.substr(1);
      }
    }
  }
}