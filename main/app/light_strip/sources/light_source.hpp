#pragma once
#include "infrastructure/array_view.hpp"
#include "app/light_strip/settings/light_strip_settings.hpp"

namespace mesh::app::light_strip::sources
{
  class light_source
  {
  public:
    light_source(const settings::light_strip_settings* settings);
    virtual ~light_source() = default;

    virtual settings::light_source_type type() const = 0;
    virtual const settings::light_source_settings* get_settings() const = 0;

    virtual void on_settings_changed() { };
    virtual void fill(infrastructure::array_view<graphics::color_rgb> &pixels) = 0;

  protected:
    const settings::light_strip_settings* _settings;
  };
}