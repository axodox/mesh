#pragma once
#include "light_source.hpp"
#include "infrastructure/logger.hpp"

namespace mesh::app::light_strip::sources
{
  void enable_usb_lamp_array();
  bool is_in_usb_lamp_array_autonomous_mode();

  class usb_source : public light_source
  {
    static constexpr infrastructure::logger _logger{"usb_source"};

  public:
    usb_source(light_strip_context& context);

    virtual settings::light_source_type type() const override;
    virtual const settings::light_source_settings* get_settings() const override;
    virtual void fill(std::span<graphics::color_rgb> pixels) override;
  };
}