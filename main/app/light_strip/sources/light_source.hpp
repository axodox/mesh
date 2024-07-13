#pragma once
#include "infrastructure/array_view.hpp"
#include "app/light_strip/light_strip_context.hpp"

namespace mesh::app::light_strip::sources
{
  class light_source
  {
  public:
    light_source(light_strip_context& context);
    virtual ~light_source() = default;

    virtual settings::light_source_type type() const = 0;
    virtual const settings::light_source_settings* get_settings() const = 0;

    virtual void on_settings_changed() { };
    virtual void fill(std::span<graphics::color_rgb> pixels) = 0;

    virtual std::span<const uint8_t> gains();

  protected:
    light_strip_context& _context;
  };
}