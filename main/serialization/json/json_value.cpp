#include "json_value.hpp"
#include "json_null.hpp"
#include "json_boolean.hpp"
#include "json_string.hpp"
#include "json_array.hpp"
#include "json_object.hpp"
#include "json_number.hpp"

using namespace std;

namespace mesh::serialization::json
{
  std::string json_value::to_string() const
  {
    stringstream stream;
    to_string(stream);
    return stream.str();
  }

  std::unique_ptr<json_value> json_value::from_string(const char* text)
  {
    string_view view{ text };
    return from_string(view);
  }

  std::unique_ptr<json_value> json_value::from_string(std::string_view& text)
  {
    json_skip_whitespace(text);

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

  std::unique_ptr<json_value> json_value::from_string(const std::string& text)
  {
    std::string_view view = text;
    return from_string(view);
  }

  void json_skip_whitespace(std::string_view& text)
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
}