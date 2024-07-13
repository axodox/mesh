#include "light_strip_controller.hpp"
#include "infrastructure/dependencies.hpp"
#include "infrastructure/error.hpp"
#include "serialization/json.hpp"
#include "app/light_strip/processors/brightness_processor.hpp"
#include "storage/file_io.hpp"

using namespace std;
using namespace std::chrono;
using namespace mesh::graphics;
using namespace mesh::peripherals;
using namespace mesh::infrastructure;
using namespace mesh::threading;
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
    _logger.log_message(log_severity::info, "Starting...");
    load_settings();
    _brightness_processor = make_unique<brightness_processor>(&settings);
    initialize_source();
    _thread = make_unique<task>([&] { worker(); }, task_affinity::core_0, task_priority::maximum, "light_strip");
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

  const sources::light_source* light_strip_controller::source() const
  {
    return _source.get();
  }

  void light_strip_controller::worker()
  {
    vector<color_rgb> lights;
    span<color_rgb> lights_view;
    while(!_isDisposed)
    {
      auto now = steady_clock::now();

      //Resize buffer as needed
      if(lights.size() != settings.device.light_count)
      {
        lights.resize(settings.device.light_count);
        lights_view = lights;
      }

      //Fill buffer
      {
        lock_guard<mutex> lock(_mutex);
        _source->fill(lights_view);
      }

      //Apply brightness, gamma and dithering
      _brightness_processor->process(lights_view);

      //Write pixels
      _strip->push_pixels(lights_view);

      //Save settings if needed
      save_settings();

      //Wait for next frame
      this_thread::sleep_until(now + settings.device.interval);
    }
  }

  void light_strip_controller::apply_brightness_settings()
  {
    _brightness_processor->on_settings_changed();
    _last_settings_change = steady_clock::now();

    _logger.log_message(log_severity::info, "Applied lighting brightness settings.");
  }

  void light_strip_controller::apply_source_settings()
  {
    initialize_source();
    _last_settings_change = steady_clock::now();
  }

  void light_strip_controller::apply_device_settings()
  {
    _last_settings_change = steady_clock::now();
  }

  void light_strip_controller::load_settings()
  {
    auto text = file_io::read_all_text(_settings_path);    
    auto json = json_value::from_string(text);
    
    if(json_serializer<light_strip_settings>::from_json(json, settings))
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

    auto json = json_serializer<light_strip_settings>::to_json(settings);
    file_io::write_all_text(_settings_path, json->to_string());

    _logger.log_message(log_severity::info, "Saved settings to %s.", _settings_path);
  }

  void light_strip_controller::initialize_source()
  {
    if(!_source || settings.source_type != _source->type())
    {
      lock_guard<mutex> lock(_mutex);
      switch(settings.source_type)
      {
      case light_source_type::empty_source:
        _source = make_unique<empty_source>(*this);
        break;
      case light_source_type::static_source:
        _source = make_unique<static_source>(*this);
        break;
      case light_source_type::rainbow_source:
        _source = make_unique<rainbow_source>(*this);
        break;
      case light_source_type::udp_source:
        _source = make_unique<udp_source>(*this);
        break;
      case light_source_type::uart_source:
        _source = make_unique<uart_source>(*this);
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