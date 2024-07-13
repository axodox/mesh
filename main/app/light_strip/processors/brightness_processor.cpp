#include "brightness_processor.hpp"
#include <cmath>

using namespace std;
using namespace mesh::numerics;
using namespace mesh::graphics;

namespace mesh::app::light_strip::processors
{
  brightness_processor::brightness_processor(const settings::light_strip_settings* settings) :
    color_processor(settings),
    _gamma_correction(settings->brightness_processor)
  {
  }

  void brightness_processor::on_settings_changed()
  {
    _gamma_correction.configure(_settings->brightness_processor);
  }

  void brightness_processor::process(std::span<graphics::color_rgb> pixels, std::span<const uint8_t> gains)
  {
    _gamma_correction.correct_gamma(pixels, gains);
  }
}