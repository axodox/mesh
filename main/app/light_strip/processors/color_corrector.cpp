#include "color_corrector.hpp"
#include "app\light_strip\helpers\serialization.hpp"
#include <cmath>

using namespace std;
using namespace mesh::json;
using namespace mesh::numerics;
using namespace mesh::graphics;

namespace mesh::app::light_strip::processors
{
  std::unique_ptr<json_value> color_corrector_settings::to_json() const
  {
    using namespace mesh::app::light_strip::helpers;
    auto object = make_unique<json_object>();
    object->set_value("gamma", float3::to_json(gamma));
    object->set_value("saturation", saturation);
    object->set_value("brightness", brightness);
    object->set_value("max_brightness", max_brightness);
    return object;
  }

  void color_corrector_settings::from_json(const json_value* value)
  {
    using namespace mesh::app::light_strip::helpers;
    if (value->type() != json_type::object) return;

    auto object = static_cast<const json_object*>(value);
    gamma = float3::from_json(object->at("gamma").get());
    object->get_value("saturation", saturation);
    object->get_value("brightness", brightness);
    object->get_value("max_brightness", max_brightness);
  }

  color_corrector::color_corrector() :
    _brightness(1.f),
    _saturation(1.f),
    _max_brightness(1.f),
    _gamma(1.6f, 1.5f, 1.6f)
  {
    rebuild_gamma();
  }

  void color_corrector::apply_settings(const color_corrector_settings* settings)
  {
    _brightness = settings->brightness;
    _saturation = settings->saturation;
    _max_brightness = settings->max_brightness;
    _gamma = settings->gamma;
    rebuild_gamma();
  }

  void color_corrector::process(infrastructure::array_view<graphics::color_rgb>& pixels)
  {
    //Brightness limit & saturation adjustment
    auto saturation = 1.f / (4.f * _saturation);

    float3 sum{};
    for (auto& color : pixels)
    {
      if (saturation != 1.f)
      {
        color_hsl hsl = color;
        hsl.s = saturation < 10 ? pow(hsl.s, saturation) : 0.f;
        color = hsl;
      }

      sum += float3{
        _gamma_mapping_r[color.r],
        _gamma_mapping_g[color.g],
        _gamma_mapping_b[color.b],
      };
    }

    auto brightness = (sum.x + sum.y + sum.z) / 3.f / 255.f / pixels.size();
    auto factor = brightness > _max_brightness ? _max_brightness / brightness : 1.f;

    //Gamma correct colors and perform dithering
    float3 target{}, error{}, actual{}, correction{};
    for (auto& color : pixels)
    {
      target = float3{
        _gamma_mapping_r[color.r],
        _gamma_mapping_g[color.g],
        _gamma_mapping_b[color.b]
      } * factor;

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

  void color_corrector::rebuild_gamma()
  {
    _gamma_mapping_r = make_gamma(_gamma.x, _brightness);
    _gamma_mapping_g = make_gamma(_gamma.y, _brightness);
    _gamma_mapping_b = make_gamma(_gamma.z, _brightness);
  }
}