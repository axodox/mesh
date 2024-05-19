#pragma once
#include "light_source.hpp"
#include "threading/task.hpp"
#include "threading/event.hpp"
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
    virtual void fill(std::span<graphics::color_rgb> pixels) override;

  private:
    std::mutex _mutex;
    std::span<graphics::color_rgb> _buffer{};
    threading::task _thread;

    void receive_data();
  };
}