#pragma once
#include "graphics/colors.hpp"
#include "infrastructure/array_view.hpp"
#include "app/light_strip/settings/light_strip_settings.hpp"

namespace mesh::app::light_strip::processors
{
  class color_processor
  {
  public:
    color_processor(const settings::light_strip_settings* settings);
    virtual ~color_processor() = default;

    virtual void on_settings_changed() { };
    virtual void process(std::span<graphics::color_rgb> pixels, std::span<const uint8_t> gains) = 0;

  protected:
    const settings::light_strip_settings* _settings;
  };
}