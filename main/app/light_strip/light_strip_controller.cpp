#include "light_strip_controller.hpp"
#include "infrastructure/dependencies.hpp"
#include "infrastructure/error.hpp"
#include "serialization/json.hpp"
#include "networking/http_server.hpp"
#include "app/light_strip/processors/brightness_processor.hpp"
#include "storage/file_io.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::graphics;
using namespace mesh::peripherals;
using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::storage;
using namespace mesh::serialization::json;
using namespace mesh::app::light_strip::sources;
using namespace mesh::app::light_strip::processors;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip
{
  light_strip_controller::light_strip_controller() :
    _strip(dependencies.resolve<peripherals::led_strip>())
  {
    load_settings();
    _brightness_processor = make_unique<brightness_processor>(&_settings);
    initialize_source();
    _thread = make_unique<task>([&] { worker(); }, task_affinity::core_0, task_priority::maximum);

    _logger.log_message(log_severity::info, "Starting...");
    auto server = dependencies.resolve<http_server>();
    server->add_handler(http_query_method::get, _root_uri, [&](http_query &query) { on_get(query); });
    server->add_handler(http_query_method::post, _root_uri, [&](http_query &query) { on_post(query); });
    _logger.log_message(log_severity::info, "Started.");
  }

  light_strip_controller::~light_strip_controller()
  {
    if(_isDisposed) return;

    _logger.log_message(log_severity::info, "Stopping...");
    _isDisposed = true;
    _thread.reset();
    _logger.log_message(log_severity::info, "Stopped.");
  }

  void light_strip_controller::worker()
  {
    vector<color_rgb> lights;
    array_view<color_rgb> lights_view;
    while(!_isDisposed)
    {
      auto now = steady_clock::now();

      if(lights.size() != _settings.device.light_count)
      {
        lights.resize(_settings.device.light_count);
        lights_view = lights;
      }

      {
        lock_guard<mutex> lock(_mutex);
        _source->fill(lights_view);
      }

      _brightness_processor->process(lights_view);
      _strip->push_pixels(lights_view);

      save_settings();
      this_thread::sleep_until(now + _settings.device.interval);
    }
  }

  void light_strip_controller::on_get(networking::http_query &query)
  {
    if(strcmp(query.uri(), _mode_uri) == 0)
    {
      auto json = json_serializer<unique_ptr<light_source_settings>>::to_json(_source->get_settings());
      query.return_text(json->to_string());
    }
    else if(strcmp(query.uri(), _brightness_uri) == 0)
    {
      auto json = json_serializer<brightness_processor_settings>::to_json(_settings.brightness_processor);
      query.return_text(json->to_string());
    }
    else if(strcmp(query.uri(), _device_uri) == 0)
    {
      auto json = json_serializer<device_settings>::to_json(_settings.device);
      query.return_text(json->to_string());
    }
  }

  void light_strip_controller::on_post(networking::http_query &query)
  {
    if(strcmp(query.uri(), _mode_uri) == 0)
    {
      on_post_mode(query);
    }
    else if(strcmp(query.uri(), _brightness_uri) == 0)
    {
      on_post_brightness(query);
    }
    else if(strcmp(query.uri(), _device_uri) == 0)
    {
      on_post_device(query);
    }

    _last_settings_change = steady_clock::now();
  }

  void light_strip_controller::on_post_brightness(networking::http_query &query)
  {
    auto body = query.get_text();
    auto json = json_value::from_string(body);
    if(json_serializer<brightness_processor_settings>::from_json(json, _settings.brightness_processor))
    {
      _brightness_processor->on_settings_changed();
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
    if(json_serializer<unique_ptr<light_source_settings>>::from_json(json, settings))
    {
      _settings.source_type = settings->type();
      switch(settings->type())
      {
        case light_source_type::static_source:
          _settings.static_source = static_cast<const static_source_settings&>(*settings);
        break;
        case light_source_type::rainbow_source:
          _settings.rainbow_source = static_cast<const rainbow_source_settings&>(*settings);
        break;
      }

      initialize_source();
    }
    else
    {
      _logger.log_message(log_severity::warning, "Failed to parse lighting mode settings.");
    }
  }

  void light_strip_controller::on_post_device(networking::http_query &query)
  {
    auto body = query.get_text();
    auto json = json_value::from_string(body);
    json_serializer<device_settings>::from_json(json, _settings.device);
  }

  void light_strip_controller::load_settings()
  {
    auto text = file_io::read_all_text(_settings_path);    
    auto json = json_value::from_string(text);
    
    if(json_serializer<light_strip_settings>::from_json(json, _settings))
    {
      _logger.log_message(log_severity::info, "Loaded settings from %s.", _settings_path);
    }
    else
    {
      _logger.log_message(log_severity::warning, "Failed to load settings from %s. Using defaults.", _settings_path);
    }
  }

  void light_strip_controller::save_settings()
  {
    if(!_last_settings_change.has_value() || steady_clock::now() - _last_settings_change.value() < _settings_save_delay) return;
    _last_settings_change.reset();

    auto json = json_serializer<light_strip_settings>::to_json(_settings);
    file_io::write_all_text(_settings_path, json->to_string());

    _logger.log_message(log_severity::info, "Saved settings to %s.", _settings_path);
  }

  void light_strip_controller::initialize_source()
  {
    if(!_source || _settings.source_type != _source->type())
    {
      lock_guard<mutex> lock(_mutex);
      switch(_settings.source_type)
      {
        case light_source_type::static_source:
          _source = make_unique<static_source>(&_settings);
        break;
        case light_source_type::rainbow_source:
          _source = make_unique<rainbow_source>(&_settings);
        break;
      }
      _logger.log_message(log_severity::info, "Lighting mode changed.");
    }
    else
    {
      _source->on_settings_changed();
      _logger.log_message(log_severity::info, "Applied lighting mode settings.");
    }
  }
}