#pragma once
#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  class static_source : public light_source
  {
  public:
    using light_source::light_source;

    virtual settings::light_source_type type() const override;
    virtual const settings::light_source_settings* get_settings() const override;
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) override;
  };
}