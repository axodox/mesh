#include "json_number.hpp"

using namespace std;

namespace mesh::serialization::json
{
  void json_number::to_string(std::stringstream& stream) const
  {
    stream << value;
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
}