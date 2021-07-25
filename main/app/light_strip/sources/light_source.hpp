#pragma once
#include "graphics/colors.hpp"
#include "infrastructure/array_view.hpp"

namespace mesh::app::light_strip::sources
{
  enum class light_source_type
  {
    static_source,
    rainbow_source
  };

  struct light_source_settings
  {
    virtual ~light_source_settings() = default;
    virtual light_source_type source_type() const = 0;
  };

  class light_source
  {
  public:
    virtual light_source_type source_type() const = 0;
    virtual ~light_source() = default;

    virtual void apply_settings(const light_source_settings *settings) {}
    virtual void fill(infrastructure::array_view<graphics::color_rgb> &pixels) = 0;
  };
}