#pragma once
#include "infrastructure/array_view.hpp"
#include "app/light_strip/light_strip_context.hpp"

namespace mesh::app::light_strip::sources
{
  struct light_source_properties
  {
    bool steady_frame_source = true;
  };

  class light_source
  {
  public:
    light_source(light_strip_context& context);
    virtual ~light_source() = default;

    virtual settings::light_source_type type() const = 0;
    virtual const settings::light_source_settings* get_settings() const = 0;

    virtual void on_settings_changed() { };
    virtual void fill(infrastructure::array_view<graphics::color_rgb> &pixels) = 0;

    const light_source_properties& properties() const;

  protected:
    light_strip_context& _context;
    light_source_properties _properties{};
  };
}