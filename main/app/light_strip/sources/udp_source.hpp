#pragma once
#include "light_source.hpp"
#include "threading/task.hpp"
#include "infrastructure/logger.hpp"

namespace mesh::app::light_strip::sources
{
  class udp_source : public light_source
  {
    static constexpr infrastructure::logger _logger{"udp_source"};

  public:
    udp_source(light_strip_context& context);

    virtual settings::light_source_type type() const override;
    virtual const settings::light_source_settings* get_settings() const override;
    virtual void fill(infrastructure::array_view<graphics::color_rgb>& pixels) override;

  private:
    std::unique_ptr<threading::task> _thread;
    std::mutex _mutex;
    infrastructure::array_view<graphics::color_rgb> _buffer{};

    void receive_data();
  };
}