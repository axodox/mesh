#include "light_strip_controller.hpp"
#include "infrastructure/dependencies.hpp"
#include "infrastructure/error.hpp"
#include "networking/http_server.hpp"
#include "app/light_strip/sources/static_source.hpp"
#include "app/light_strip/sources/rainbow_source.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::graphics;
using namespace mesh::peripherals;
using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::json;
using namespace mesh::app::light_strip::sources;

namespace mesh::app::light_strip
{
  const std::chrono::milliseconds light_strip_controller::interval = milliseconds(16);

  light_strip_controller::light_strip_controller() :
    _light_count(4),
    _strip(dependencies.resolve<peripherals::led_strip>()),
    //_source(make_unique<static_source>()),
    _source(make_unique<rainbow_source>()),
    _thread([&] { worker(); }, configMAX_PRIORITIES )
  {
    _logger.log_message(log_severity::info, "Starting...");
    auto server = dependencies.resolve<http_server>();
    server->add_handler(http_query_method::post, "/api/light_strip/mode", [&](http_query &query) {
      on_post(query); 
    });
    _logger.log_message(log_severity::info, "Started.");
  }

  light_strip_controller::~light_strip_controller()
  {
    if(_isDisposed) return;

    _logger.log_message(log_severity::info, "Stopping...");
    _isDisposed = true;
    _thread.close();
    _logger.log_message(log_severity::info, "Stopped.");
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

      {
        lock_guard<mutex> lock(_mutex);
        _source->fill(lights_view);
      }
      _strip->push_pixels(lights_view);

      this_thread::sleep_until(now + interval);
    }
  }

  void light_strip_controller::on_post(networking::http_query &query)
  {
    auto body = query.get_text();
    auto settings = light_source_settings::from_string(body);
    if(!settings) return;

    if(settings->source_type() != _source->source_type())
    {
      lock_guard<mutex> lock(_mutex);
      switch(settings->source_type())
      {
        case light_source_type::static_source:
          _source = make_unique<static_source>();
        break;
        case light_source_type::rainbow_source:
          _source = make_unique<rainbow_source>();
        break;
      }
      _logger.log_message(log_severity::info, "Switched lighting mode to %s.", settings->type_name().c_str());
    }

    _source->apply_settings(settings.get());
    _logger.log_message(log_severity::info, "Applied settings.");
  }
}