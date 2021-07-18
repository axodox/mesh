#include "light_strip_controller.hpp"
#include "infrastructure/dependencies.hpp"
#include "app/light_strip/sources/static_source.hpp"
#include "app/light_strip/sources/rainbow_source.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::graphics;
using namespace mesh::peripherals;
using namespace mesh::infrastructure;
using namespace mesh::app::light_strip::sources;

namespace mesh::app::light_strip
{
  const std::chrono::milliseconds light_strip_controller::interval = milliseconds(16);

  light_strip_controller::light_strip_controller() :
    _light_count(4),
    _strip(dependencies.resolve<peripherals::led_strip>()),
    //_source(make_unique<static_source>()),
    _source(make_unique<rainbow_source>()),
    _thread([&]{ worker(); })
  { }

  light_strip_controller::~light_strip_controller()
  {
    if(_isDisposed) return;

    _isDisposed = true;
    if(_thread.joinable()) _thread.join();
  }

  void light_strip_controller::worker()
  {
    vector<color_rgb> lights;
    array_view<color_rgb> lights_view;
    while(!_isDisposed)
    {
      auto now = steady_clock::now();

      if(lights.size() != _light_count)
      {
        lights.resize(_light_count);
        lights_view = lights;
      }

      _source->fill(lights_view);
      _strip->push_pixels(lights_view);

      this_thread::sleep_until(now + interval);
    }
  }
}