#pragma once
#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  class empty_source : public light_source
  {
  public:
    empty_source(light_strip_context& context);

    virtual settings::light_source_type type() const override;
    virtual const settings::light_source_settings* get_settings() const override;
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) override;
  };
}