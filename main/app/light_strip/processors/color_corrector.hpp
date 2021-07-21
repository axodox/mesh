#pragma once
#include "color_processor.hpp"
#include "numerics/float3.hpp"
#include "infrastructure/json.hpp"
#include <array>
#include <vector>

namespace mesh::app::light_strip::processors
{
  struct color_corrector_settings : json::json_serializable<color_corrector_settings>
  {
    numerics::float3 gamma;
    float brightness;
    float max_brightness;

    virtual std::unique_ptr<json_value> to_json() const override;
    virtual void from_json(const json_value* value) override;
  };

  class color_corrector : public color_processor
  {
  public:
    color_corrector();

    void apply_settings(const color_corrector_settings* settings);
    virtual void process(infrastructure::array_view<graphics::color_rgb>& pixels) override;

  private:
    float _brightness;
    float _max_brightness;
    numerics::float3 _gamma;
    std::vector<numerics::float3> _buffer;

    std::array<float, 256> _gamma_mapping_r;
    std::array<float, 256> _gamma_mapping_g;
    std::array<float, 256> _gamma_mapping_b;

    void rebuild_gamma();
  };
}