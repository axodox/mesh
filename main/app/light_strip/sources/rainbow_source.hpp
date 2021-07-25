#pragma once
#include "light_source.hpp"
#include <math.h>

namespace mesh::app::light_strip::sources
{
  struct rainbow_source_settings : public light_source_settings
  {
    inline static const char* type_name = "rainbow";

    uint8_t spatial_frequency = 1;
    float angular_velocity = float(M_PI);

    virtual light_source_type type() const override;
  };

  class rainbow_source : public light_source
  {
  public:
    uint8_t spatial_frequency;
    float angular_velocity;

    rainbow_source();

    virtual light_source_type type() const override;

    virtual void apply_settings(const light_source_settings* settings) override;
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) override;

  private:
    float _angle = 0.f;
  };
}