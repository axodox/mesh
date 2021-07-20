#pragma once
#include "graphics/colors.hpp"
#include "infrastructure/array_view.hpp"
#include "infrastructure/json.hpp"

namespace mesh::app::light_strip::sources
{
  enum class light_source_type
  {
    static_source,
    rainbow_source
  };

  struct static_source_settings;
  struct rainbow_source_settings;

  struct light_source_settings : public json::json_serializable<light_source_settings, static_source_settings, rainbow_source_settings>
  {
    virtual light_source_type source_type() const = 0;
  };

  class light_source
  {
  public:
    virtual light_source_type source_type() const = 0;
    virtual ~light_source() = default;    

    virtual void apply_settings(const light_source_settings* settings) { }
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) = 0;
  };
}