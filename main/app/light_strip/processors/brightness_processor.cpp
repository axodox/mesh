#include "brightness_processor.hpp"
#include <cmath>

using namespace std;
using namespace mesh::numerics;
using namespace mesh::graphics;

namespace mesh::app::light_strip::processors
{
  brightness_processor::brightness_processor()
  {
    brightness_processor_settings default_settings{};
    apply_settings(&default_settings);
    rebuild_gamma();
  }

  void brightness_processor::apply_settings(const brightness_processor_settings* settings)
  {
    _brightness = settings->brightness;
    _max_brightness = settings->max_brightness;
    _gamma = settings->gamma;
    rebuild_gamma();
  }

  void brightness_processor::process(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    //Prepare buffer
    if(_buffer.size() != pixels.size())
    {
      _buffer.resize(pixels.size());
    }

    //Brightness limit
    auto buffer = _buffer.data();
    float3 sum{};
    for (auto& color : pixels)
    {
      *buffer = float3{
        _gamma_mapping_r[color.r],
        _gamma_mapping_g[color.g],
        _gamma_mapping_b[color.b],
      };

      sum += *buffer;
      buffer++;
    }

    auto brightness = (sum.x + sum.y + sum.z) / 3.f / 255.f / pixels.size();
    auto factor = brightness > _max_brightness ? _max_brightness / brightness : 1.f;

    //Gamma correct colors and perform dithering
    buffer = _buffer.data();
    float3 target{}, error{}, actual{}, correction{};
    for (auto& color : pixels)
    {
      target = *buffer++ * factor;

      actual = target.round();
      error += target - actual;

      correction = error.round();
      actual += correction;
      error -= correction;

      color.r = uint8_t(actual.x);
      color.g = uint8_t(actual.y);
      color.b = uint8_t(actual.z);
    }
  }

  void brightness_processor::rebuild_gamma()
  {
    _gamma_mapping_r = make_gamma(_gamma.x, _brightness);
    _gamma_mapping_g = make_gamma(_gamma.y, _brightness);
    _gamma_mapping_b = make_gamma(_gamma.z, _brightness);
  }
}