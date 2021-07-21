#pragma once
#include "color_processor.hpp"
#include "numerics/float3.hpp"
#include "infrastructure/json.hpp"
#include <array>
#include <vector>

namespace mesh::app::light_strip::processors
{
  struct brightness_processor_settings : json::json_serializable<brightness_processor_settings>
  {
    numerics::float3 gamma{1.6f, 1.5f, 1.6f};
    float brightness = 1.f;
    float max_brightness = 0.7f;

    virtual std::unique_ptr<json_value> to_json() const override;
    virtual void from_json(const json_value* value) override;
  };

  class brightness_processor : public color_processor
  {
  public:
    brightness_processor();

    void apply_settings(const brightness_processor_settings* settings);
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