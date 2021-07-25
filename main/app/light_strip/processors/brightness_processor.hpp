#pragma once
#include "color_processor.hpp"
#include "numerics/float3.hpp"
#include <array>
#include <vector>

namespace mesh::app::light_strip::processors
{
  struct brightness_processor_settings
  {
    numerics::float3 gamma{1.6f, 1.5f, 1.6f};
    float brightness = 1.f;
    float max_brightness = 0.7f;
  };

  class brightness_processor : public color_processor
  {
  public:
    brightness_processor();

    const brightness_processor_settings* get_settings() const;
    void apply_settings(const brightness_processor_settings* settings);

    virtual void process(infrastructure::array_view<graphics::color_rgb>& pixels) override;

  private:
    brightness_processor_settings _settings{};
    std::vector<numerics::float3> _buffer;

    std::array<float, 256> _gamma_mapping_r;
    std::array<float, 256> _gamma_mapping_g;
    std::array<float, 256> _gamma_mapping_b;

    void rebuild_gamma();
  };
}