#include "light_strip_http_interface.hpp"
#include "infrastructure/dependencies.hpp"
#include "networking/http_server.hpp"
#include "serialization/json.hpp"

using namespace std;
using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::serialization::json;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip
{
  light_strip_http_interface::light_strip_http_interface()
  {
    _controller = dependencies.resolve<light_strip_controller>();

    auto server = dependencies.resolve<http_server>();
    server->add_handler(http_query_method::get, _root_uri, [&](http_query &query) { on_get(query); });
    server->add_handler(http_query_method::post, _root_uri, [&](http_query &query) { on_post(query); });
    _logger.log_message(log_severity::info, "Enabled.");
  }

  void light_strip_http_interface::on_get(networking::http_query &query)
  {
    if(strcmp(query.uri(), _mode_uri) == 0)
    {
      auto json = json_serializer<unique_ptr<light_source_settings>>::to_json(_controller->source()->get_settings());
      query.return_text(json->to_string());
    }
    else if(strcmp(query.uri(), _brightness_uri) == 0)
    {
      auto json = json_serializer<brightness_processor_settings>::to_json(_controller->settings.brightness_processor);
      query.return_text(json->to_string());
    }
    else if(strcmp(query.uri(), _device_uri) == 0)
    {
      auto json = json_serializer<device_settings>::to_json(_controller->settings.device);
      query.return_text(json->to_string());
    }
  }

  void light_strip_http_interface::on_post(networking::http_query &query)
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
  }

  void light_strip_http_interface::on_post_brightness(networking::http_query &query)
  {
    auto body = query.get_text();
    auto json = json_value::from_string(body);
    if(json_serializer<brightness_processor_settings>::from_json(json, _controller->settings.brightness_processor))
    {
      _controller->apply_brightness_settings();
    }
    else
    {
      _logger.log_message(log_severity::warning, "Failed to parse lighting brightness settings.");
    }
  }

  void light_strip_http_interface::on_post_mode(networking::http_query &query)
  {
    auto body = query.get_text();
    auto json = json_value::from_string(body);

    unique_ptr<light_source_settings> source_settings;
    if(json_serializer<unique_ptr<light_source_settings>>::from_json(json, source_settings, { 
      &_controller->settings.static_source, 
      &_controller->settings.rainbow_source, 
      &_controller->settings.udp_source }))
    {
      _controller->settings.source_type = source_settings->type();
      switch(source_settings->type())
      {
      case light_source_type::empty_source:
        _controller->settings.empty_source = static_cast<const empty_source_settings&>(*source_settings);
        break;
      case light_source_type::static_source:
        _controller->settings.static_source = static_cast<const static_source_settings&>(*source_settings);
        break;
      case light_source_type::rainbow_source:
        _controller->settings.rainbow_source = static_cast<const rainbow_source_settings&>(*source_settings);
        break;
      case light_source_type::udp_source:
        _controller->settings.udp_source = static_cast<const udp_source_settings&>(*source_settings);
        break;
      case light_source_type::uart_source:
        _controller->settings.uart_source = static_cast<const uart_source_settings&>(*source_settings);
        break;
      }

      _controller->apply_source_settings();
    }
    else
    {
      _logger.log_message(log_severity::warning, "Failed to parse lighting mode settings.");
    }
  }

  void light_strip_http_interface::on_post_device(networking::http_query &query)
  {
    auto body = query.get_text();
    auto json = json_value::from_string(body);
    json_serializer<device_settings>::from_json(json, _controller->settings.device);
    _controller->apply_device_settings();
  }
}