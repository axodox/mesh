#include "json.hpp"
#include <sstream>
#include <optional>
//#include <charconv>

using namespace std;

namespace mesh::json
{
  void skip_whitespace(std::string_view& text)
  {
    for (auto& character : text)
    {
      switch (character)
      {
      case ' ':
      case '\r':
      case '\n':
      case '\t':
        //Skip white space
        break;
      default:
        text = text.substr(size_t(&character - text.data()));
        return;
      }
    }
  }


  std::unique_ptr<json_value> json_value::from_string(std::string_view& text)
  {
    skip_whitespace(text);

    if (text.empty()) return nullptr;

    switch (text.front())
    {
    case 'n':
      return json_null::from_string(text);
    case 't':
    case 'f':
      return json_boolean::from_string(text);
    case '"':
      return json_string::from_string(text);
    case '[':
      return json_array::from_string(text);
    case '{':
      return json_object::from_string(text);
    default:
      return json_number::from_string(text);
    }
  }

  inline std::unique_ptr<json_value> json_value::from_string(const std::string& text)
  {
    std::string_view view = text;
    return from_string(view);
  }

  json_type json_null::type() const
  {
    return json_type::null;
  }

  std::string json_null::to_string() const
  {
    return "null";
  }

  std::unique_ptr<json_null> json_null::from_string(std::string_view& text)
  {
    if (text.compare(0, 4, "null") == 0)
    {
      text = text.substr(4);
      return make_unique<json_null>();
    }
    else
    {
      return nullptr;
    }
  }

  std::string json_boolean::to_string() const
  {
    return value ? "true" : "false";
  }

  std::unique_ptr<json_boolean> json_boolean::from_string(std::string_view& text)
  {
    if (text.compare(0, 4, "true") == 0)
    {
      text = text.substr(4);
      return make_unique<json_boolean>(true);
    }
    else if(text.compare(0, 5, "false") == 0)
    {
      text = text.substr(5);
      return make_unique<json_boolean>(false);
    }
    else
    {
      return nullptr;
    }
  }

  std::string json_number::to_string() const
  {
    auto result = std::to_string(value);
    auto dot_offset = result.find_last_of('.');
    if (dot_offset != string::npos)
    {
      auto dot = result.data() + dot_offset;
      size_t zero_offset = result.length();
      char* character;
      for (character = &result.back(); character > dot; character--)
      {
        if (*character == '0')
        {
          zero_offset--;
        }
        else
        {
          break;
        }
      }
      if (*character == '.') zero_offset--;
      result = result.erase(zero_offset);
    }
    return result;
  }

  std::unique_ptr<json_number> json_number::from_string(std::string_view& text)
  {
    //Xtensa GCC too old for this :(
    /*double number;
    auto result = std::from_chars(text.data(), text.data() + text.size(), number);
    if (result.ec == errc{})
    {
      text = text.substr(size_t(result.ptr - text.data()));
      return make_unique<json_number>(number);
    }
    else
    {
      return nullptr;
    }*/
    char* end;
    auto number = strtod(text.data(), &end);
    text = text.substr(size_t(end - text.data()));
    return make_unique<json_number>(number);
  }

  std::string json_string::to_string() const
  {
    std::stringstream result;
    result << "\"";
    for (auto character : value)
    {
      switch (character)
      {
      case '"':
        result << "\\\"";
        break;
      case '\r':
        result << "\\r";
        break;
      case '\n':
        result << "\\n";
        break;
      case '\t':
        result << "\\t";
        break;
      case '\0':
        result << "\\0";
        break;
      default:
        result << character;
        break;
      }
    }
    result << "\"";
    return result.str();
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
      else if(character == '\\')
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

  std::unique_ptr<json_value>& json_array::operator[](size_t index)
  {
    return value[index];
  }

  const std::unique_ptr<json_value>& json_array::operator[](size_t index) const
  {
    return value[index];
  }

  std::string json_array::to_string() const
  {
    std::stringstream result;

    bool is_first = true;
    result << "[";
    for (auto& item : value)
    {
      if (is_first)
      {
        is_first = false;
      }
      else
      {
        result << ",";
      }

      if (item)
      {
        result << item->to_string();
      }
      else
      {
        result << "null";
      }
    }
    result << "]";

    return result.str();
  }

  std::unique_ptr<json_array> json_array::from_string(std::string_view& text)
  {
    if (text.empty() || text[0] != '[') return nullptr;

    vector<unique_ptr<json_value>> items;
    text = text.substr(1);
    while (true)
    {
      skip_whitespace(text);
      if (text.empty()) return nullptr;
      if (text[0] == ']')
      {
        text = text.substr(1);
        return make_unique<json_array>(move(items));
      }

      auto item = json_value::from_string(text);
      if (!item) return nullptr;      
      items.push_back(move(item));

      skip_whitespace(text);
      if (!text.empty() && text[0] == ',')
      {
        text = text.substr(1);
      }
    }
  }

  std::unique_ptr<json_value>& json_object::operator[](const std::string& property)
  {
    return at(property);
  }

  const std::unique_ptr<json_value>& json_object::operator[](const std::string& property) const
  {
    return at(property);
  }

  std::unique_ptr<json_value>& json_object::at(const std::string& property)
  {
    return value[property];
  }

  const std::unique_ptr<json_value>& json_object::at(const std::string& property) const
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

  std::string json_object::to_string() const
  {
    std::stringstream result;

    bool is_first = true;
    result << "{";
    for (auto& [key, value] : value)
    {
      if (is_first)
      {
        is_first = false;
      }
      else
      {
        result << ",";
      }

      result << "\"" << key << "\":";
      if (value)
      {
        result << value->to_string();
      }
      else
      {
        result << "null";
      }
    }
    result << "}";

    return result.str();
  }

  std::unique_ptr<json_object> json_object::from_string(std::string_view& text)
  {
    if (text.empty() || text[0] != '{') return nullptr;

    unordered_map<string, unique_ptr<json_value>> items;
    text = text.substr(1);
    while (true)
    {
      skip_whitespace(text);
      if (text.empty()) return nullptr;
      if (text[0] == '}')
      {
        text = text.substr(1);
        return make_unique<json_object>(move(items));
      }

      auto key = json_string::from_string(text);
      if (!key) return nullptr;

      skip_whitespace(text);
      if (text.empty() || text[0] != ':') return nullptr;
      text = text.substr(1);

      auto value = json_value::from_string(text);
      if (!value) return nullptr;

      items[key->value] = move(value);

      skip_whitespace(text);
      if (!text.empty() && text[0] == ',')
      {
        text = text.substr(1);
      }
    }
  }
}