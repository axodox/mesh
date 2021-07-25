#pragma once
#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  struct static_source_settings : public light_source_settings
  {
    graphics::color_rgb color{ 10, 10, 10 };

    virtual light_source_type source_type() const override;
  };

  class static_source : public light_source
  {
  public:
    graphics::color_rgb color;

    static_source();
    virtual light_source_type source_type() const override;

    virtual void apply_settings(const light_source_settings* settings) override;
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) override;
  };
}