#pragma once
#include "graphics/colors.hpp"
#include "infrastructure/array_view.hpp"
#include <chrono>

namespace mesh::app::light_strip::sources
{
  struct device_settings
  {
    uint32_t light_count = 4;
    std::chrono::milliseconds interval = std::chrono::milliseconds(16);
  };

  enum class light_source_type
  {
    static_source,
    rainbow_source
  };

  struct light_source_settings
  {
    virtual ~light_source_settings() = default;
    virtual light_source_type type() const = 0;
  };

  class light_source
  {
  public:
    virtual light_source_type type() const = 0;
    virtual ~light_source() = default;

    virtual void on_device_settings_changed(const device_settings& settings) { }

    virtual const light_source_settings * get_settings() const = 0;
    virtual void apply_settings(const light_source_settings *settings) = 0;

    virtual void fill(infrastructure::array_view<graphics::color_rgb> &pixels) = 0;
  };
}