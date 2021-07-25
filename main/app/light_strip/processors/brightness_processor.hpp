#pragma once
#include <array>
#include <vector>
#include "color_processor.hpp"

namespace mesh::app::light_strip::processors
{
  class brightness_processor : public color_processor
  {
  public:
    brightness_processor(const settings::light_strip_settings* settings);

    void on_settings_changed();
    virtual void process(infrastructure::array_view<graphics::color_rgb>& pixels) override;

  private:
    std::vector<numerics::float3> _buffer;

    std::array<float, 256> _gamma_mapping_r;
    std::array<float, 256> _gamma_mapping_g;
    std::array<float, 256> _gamma_mapping_b;

    void rebuild_gamma();
  };
}