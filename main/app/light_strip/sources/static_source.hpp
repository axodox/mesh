#pragma once
#include "light_source.hpp"

namespace mesh::app::light_strip::sources
{
  struct static_source_settings : public light_source_settings
  {
    inline static const char* type_name = "static";

    graphics::color_rgb color{ 10, 10, 10 };

    virtual light_source_type type() const override;
  };

  class static_source : public light_source
  {
  public:
    virtual light_source_type type() const override;

    virtual const light_source_settings * get_settings() const override;
    virtual void apply_settings(const light_source_settings* settings) override;

    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) override;

  private:
    static_source_settings _settings{};
  };
}