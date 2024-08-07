#pragma once
#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  class rainbow_source : public light_source
  {
  public:
    using light_source::light_source;

    virtual settings::light_source_type type() const override;
    virtual const settings::light_source_settings* get_settings() const override;
    virtual void fill(std::span<graphics::color_rgb> pixels) override;

  private:
    float _angle = 0.f;
  };
}