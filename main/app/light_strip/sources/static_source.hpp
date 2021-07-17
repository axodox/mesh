#pragma once
#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  struct static_source_settings : public light_source_settings
  {
    graphics::color_rgb color;

    virtual std::string type_name() const;

    virtual std::unique_ptr<json_value> to_json() const override;
    virtual void from_json(const json_value* value) override;
  };

  class static_source : public light_source
  {
  public:
    graphics::color_rgb color{ 10, 10, 10 };

    virtual void apply_settings(const light_source_settings* settings) override;
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) override;
  };
}