#pragma once
#include "graphics/colors.hpp"
#include "infrastructure/array_view.hpp"
#include "infrastructure/json.hpp"

namespace mesh::app::light_strip::sources
{
  struct static_source_settings;

  struct light_source_settings : public json::json_serializable<light_source_settings, static_source_settings>
  {
    
  };

  class light_source
  {
  public:
    virtual void apply_settings(const light_source_settings* settings) { }
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) = 0;
  };
}