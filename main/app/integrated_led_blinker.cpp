#include "integrated_led_blinker.hpp"
#include "infrastructure/dependencies.hpp"
#include "networking/http_server.hpp"

using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::peripherals;

namespace mesh::app
{
  integrated_led_blinker::integrated_led_blinker() :
    _led(dependencies.resolve<integrated_led>())
  { 
    auto server = dependencies.resolve<http_server>();
    server->add_handler(http_query_method::put, "/api/led/*", [&](http_query &query) { 
      on_api_call(query); 
    });
  }

  void integrated_led_blinker::on_api_call(networking::http_query &query)
  {
    if (strcmp(query.uri(), "/api/led/on") == 0)
    {
      _led->state(true);
    }
    else if (strcmp(query.uri(), "/api/led/off") == 0)
    {
      _led->state(false);
    }
  }
}