#pragma once
#include <array>
#include <vector>

#include "color_processor.hpp"
#include "graphics/gamma_correction.hpp"

namespace mesh::app::light_strip::processors
{
  class brightness_processor : public color_processor
  {
  public:
    brightness_processor(const settings::light_strip_settings* settings);

    void on_settings_changed();
    virtual void process(std::span<graphics::color_rgb> pixels) override;

  private:
    graphics::gamma_correction _gamma_correction;
  };
}