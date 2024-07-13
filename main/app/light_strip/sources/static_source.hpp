#pragma once
#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  class static_source : public light_source
  {
  public:
    static_source(light_strip_context& context);

    virtual settings::light_source_type type() const override;
    virtual const settings::light_source_settings* get_settings() const override;
    virtual void fill(std::span<graphics::color_rgb> pixels) override;
  };
}