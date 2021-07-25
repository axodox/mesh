#pragma once
#include "json_value.hpp"

namespace mesh::serialization::json
{
  struct json_null : public json_value
  {
    using json_value::to_string;

    virtual json_type type() const override;

    virtual void to_string(std::stringstream& stream) const override;
    static std::unique_ptr<json_null> from_string(std::string_view& text);
  };
}