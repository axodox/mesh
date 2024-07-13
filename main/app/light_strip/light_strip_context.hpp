#pragma once
#include "threading/event.hpp"
#include "app/light_strip/settings/light_strip_settings.hpp"

namespace mesh::app::light_strip
{
  struct light_strip_context
  {
    settings::light_strip_settings settings{};
  };
}