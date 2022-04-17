#pragma once
#include <vector>
#include "json_value.hpp"

namespace mesh::serialization::json
{
  struct json_array : public json_value_container<std::vector<std::unique_ptr<json_value>>, json_type::array>
  {
    using json_value_container::json_value_container;
    using json_value::to_string;

    std::unique_ptr<json_value>& operator[](size_t index);
    const std::unique_ptr<json_value>& operator[](size_t index) const;

    virtual void to_string(std::stringstream& stream) const override;
    static std::unique_ptr<json_array> from_string(std::string_view& text);
  };
}