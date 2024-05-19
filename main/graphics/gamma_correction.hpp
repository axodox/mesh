#pragma once
#include <vector>
#include <array>
#include <span>
#include "numerics/float3.hpp"
#include "colors.hpp"

namespace mesh::graphics
{
  struct gamma_correction_settings
  {
    numerics::float3 gamma = { 1.f, 1.f, 1.f };
    float brightness = 1.f;
    float max_brightness = 1.f;

    bool operator==(const gamma_correction_settings&) const = default;
    bool operator!=(const gamma_correction_settings&) const = default;
  };

  class gamma_correction
  {
  public:
    gamma_correction(const gamma_correction_settings& settings);

    void configure(const gamma_correction_settings& value);
    const gamma_correction_settings& configuration() const;

    void correct_gamma(std::span<graphics::color_rgb> pixels);

  private:
    gamma_correction_settings _settings;

    std::vector<numerics::float3> _buffer;

    std::array<float, 256> _gamma_mapping_r;
    std::array<float, 256> _gamma_mapping_g;
    std::array<float, 256> _gamma_mapping_b;
  };
}