#include "json_string.hpp"
#include <sstream>

using namespace std;

namespace mesh::serialization::json
{
  void json_string::to_string(std::stringstream& stream) const
  {
    stream << "\"";
    for (auto character : value)
    {
      switch (character)
      {
      case '"':
        stream << "\\\"";
        break;
      case '\r':
        stream << "\\r";
        break;
      case '\n':
        stream << "\\n";
        break;
      case '\t':
        stream << "\\t";
        break;
      case '\0':
        stream << "\\0";
        break;
      default:
        stream << character;
        break;
      }
    }
    stream << "\"";
  }

  std::unique_ptr<json_string> json_string::from_string(std::string_view& text)
  {
    if (text.empty()) return nullptr;

    std::stringstream result;
    bool is_first = true;
    bool is_escaping = false;
    const char* end = nullptr;
    for (auto& character : text)
    {
      if (is_first)
      {
        if (character != '"') return nullptr;
        is_first = false;
      }
      else if (is_escaping)
      {
        is_escaping = false;
        switch (character)
        {
        case '0':
          result << '\0';
          break;
        case 'r':
          result << '\r';
          break;
        case 'n':
          result << '\n';
          break;
        case 't':
          result << '\t';
          break;
        case '"':
          result << '"';
          break;
        default:
          result << character;
          break;
        }
      }
      else if (character == '\\')
      {
        is_escaping = true;
      }
      else if (character == '"')
      {
        end = &character;
        break;
      }
      else
      {
        result << character;
      }
    }

    if (!end) return nullptr;
    text = text.substr(size_t(end - text.data()) + 1);

    return make_unique<json_string>(result.str());
  }

  std::unique_ptr<json_value> json_serializer<std::string>::to_json(const std::string& value)
  {
    return make_unique<json_string>(value);
  }
  
  bool json_serializer<std::string>::from_json(const std::unique_ptr<json_value>& json, std::string& value)
  {
    if (json && json->type() == json_type::string)
    {
      value = static_cast<json_string*>(json.get())->value;
      return true;
    }
    else
    {
      return false;
    }
  }
}