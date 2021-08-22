#include "light_strip_http_interface.hpp"
#include "infrastructure/dependencies.hpp"
#include "networking/http_server.hpp"
#include "serialization/json.hpp"

using namespace std;
using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::events;
using namespace mesh::serialization::json;
using namespace mesh::app::light_strip::settings;

namespace mesh::app::light_strip
{
  light_strip_http_interface::light_strip_http_interface()
  {
    _message_serializer.add_type<brightness_processor_settings>(_brightness_key, &_controller->settings.brightness_processor);
    _message_serializer.add_multi_type<light_source_settings>(_source_key, {
      &_controller->settings.static_source, 
      &_controller->settings.rainbow_source, 
      &_controller->settings.udp_source
     });
    _message_serializer.add_type<device_settings>(_device_key, &_controller->settings.device);

    _controller = dependencies.resolve<light_strip_controller>();

    auto server = dependencies.resolve<http_server>();
    server->add_handler(http_query_method::get, _root_uri, [&](http_query &query) { on_get(query); });
    server->add_handler(http_query_method::post, _root_uri, [&](http_query &query) { on_post(query); });
    server->add_socket(_socket_uri, [&](http_socket& socket) { on_socket(socket); });
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
      apply_source_settings(source_settings.get());
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

  void light_strip_http_interface::on_socket(networking::http_socket &socket)
  {
    socket.message_received(no_revoke, member_func(this, &light_strip_http_interface::on_socket_received));
    socket.closing(no_revoke, member_func(this, &light_strip_http_interface::on_socket_closing));

    lock_guard<mutex> lock(_mutex);
    _sockets.push_back(&socket);
  }

  void light_strip_http_interface::on_socket_received(networking::http_socket* socket, const char* message)
  {
    auto json = json_value::from_string(message);

    const char* type = nullptr;
    auto value = _message_serializer.from_json(json, type);
    
    if(!value) return;

    if(type == _brightness_key)
    {
      _controller->settings.brightness_processor = *value.get<brightness_processor_settings>();
      _controller->apply_brightness_settings();
    }
    else if(type == _source_key)
    {
      apply_source_settings(value.get<unique_ptr<light_source_settings>>()->get());
    }
    else if(type == _device_key)
    {
      _controller->settings.device = *value.get<device_settings>();
      _controller->apply_device_settings();
    }
  }

  void light_strip_http_interface::on_socket_closing(networking::http_socket* socket)
  {
    lock_guard<mutex> lock(_mutex);
    _sockets.remove(socket);
  }

  void light_strip_http_interface::apply_source_settings(const settings::light_source_settings* source_settings)
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
    }

    _controller->apply_source_settings();
  }
}