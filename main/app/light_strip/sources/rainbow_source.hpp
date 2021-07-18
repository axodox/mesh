#pragma once
#include "light_source.hpp"
//#include "numerics\math_constants.hpp"
#include <math.h>

namespace mesh::app::light_strip::sources
{
  struct rainbow_source_settings : public light_source_settings
  {
    uint8_t spatial_frequency;
    float angular_velocity;

    virtual std::string type_name() const;

    virtual std::unique_ptr<json_value> to_json() const override;
    virtual void from_json(const json_value* value) override;
  };

  class rainbow_source : public light_source
  {
  public:
    uint8_t spatial_frequency = 1;
    float angular_velocity = float(M_PI);

    virtual void apply_settings(const light_source_settings* settings) override;
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) override;

  private:
    float _angle;
  };
}