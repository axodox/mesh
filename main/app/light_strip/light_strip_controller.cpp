#include "light_strip_controller.hpp"
#include "infrastructure/dependencies.hpp"
#include "infrastructure/error.hpp"
#include "serialization/json.hpp"
#include "networking/http_server.hpp"
#include "app/light_strip/processors/brightness_processor.hpp"
#include "app/light_strip/processors/json_serialization.hpp"
#include "app/light_strip/sources/static_source.hpp"
#include "app/light_strip/sources/rainbow_source.hpp"
#include "app/light_strip/sources/json_serialization.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::graphics;
using namespace mesh::peripherals;
using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::serialization::json;
using namespace mesh::app::light_strip::sources;
using namespace mesh::app::light_strip::processors;

namespace mesh::app::light_strip
{
  const std::chrono::milliseconds light_strip_controller::interval = milliseconds(16);

  light_strip_controller::light_strip_controller() :
    _light_count(4),
    _strip(dependencies.resolve<peripherals::led_strip>()),
    //_source(make_unique<static_source>()),
    _source(make_unique<rainbow_source>()),
    _brightness_processor(make_unique<brightness_processor>()),
    _thread([&] { worker(); }, task_affinity::core_0, task_priority::maximum)
  {
    _logger.log_message(log_severity::info, "Starting...");
    auto server = dependencies.resolve<http_server>();
    server->add_handler(http_query_method::post, "/api/light_strip/*", [&](http_query &query) {
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

      _brightness_processor->process(lights_view);
      _strip->push_pixels(lights_view);

      this_thread::sleep_until(now + interval);
    }
  }

  void light_strip_controller::on_post(networking::http_query &query)
  {
    if(strcmp(query.uri(), "/api/light_strip/mode") == 0)
    {
      on_post_mode(query);
    }
    else if(strcmp(query.uri(), "/api/light_strip/brightness") == 0)
    {
      on_post_brightness(query);
    }
  }

  void light_strip_controller::on_post_brightness(networking::http_query &query)
  {
    auto body = query.get_text();
    auto json = json_value::from_string(body);
    brightness_processor_settings settings;
    if(json && json_serializer<brightness_processor_settings>::from_json(json, settings))
    {
      _brightness_processor->apply_settings(&settings);
      _logger.log_message(log_severity::info, "Applied lighting brightness settings.");
    }
    else
    {
      _logger.log_message(log_severity::warning, "Failed to parse lighting brightness settings.");
    }
  }

  void light_strip_controller::on_post_mode(networking::http_query &query)
  {
    auto body = query.get_text();
    auto json = json_value::from_string(body);
    unique_ptr<light_source_settings> settings;
    if(json && json_serializer<unique_ptr<light_source_settings>>::from_json(json, settings))
    {
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
        _logger.log_message(log_severity::info, "Lighting mode changed.");
      }

      _source->apply_settings(settings.get());
      _logger.log_message(log_severity::info, "Applied lighting mode settings.");
    }
    else
    {
      _logger.log_message(log_severity::warning, "Failed to parse lighting mode settings.");
    }
  }
}