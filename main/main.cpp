#include <stdio.h>
#include <memory>
#include <exception>
#include <thread>
#include <chrono>
#include <string>

#include "infrastructure/dependencies.hpp"
#include "infrastructure/error.hpp"
#include "infrastructure/logger.hpp"
#include "networking/http_server.hpp"
#include "networking/wifi.hpp"
#include "peripherals/integrated_led.hpp"
#include "storage/embedded.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::peripherals;

define_file(favicon, "_binary_favicon_png_start", "_binary_favicon_png_end");

extern "C" void app_main()
{
  log_message(log_severity::info, "Starting...");
  dependencies.add<wifi_connection>(dependency_lifetime::singleton, []() -> unique_ptr<wifi_connection>{ return make_unique<wifi_connection>("Axodox-Ranged", "88gypARK"); } );

  try
  {
    dependencies.resolve<wifi_connection>();
    auto server = dependencies.resolve<http_server>();

    server->add_handler(http_query_method::get, "/", [](http_query& query) {
      query.return_text("<!DOCTYPE html><html><head><link rel=\"shortcut icon\" type=\"image/png\" href=\"/favicon.png\"/></head><body>Hello World!</html></body>");
    });

    server->add_handler(http_query_method::get, "/favicon.png", [](http_query& query) {
      query.return_blob("image/png", favicon);
    });

    server->add_handler(http_query_method::put, "/led/*", [](http_query& query) {
      auto& led = dependencies.resolve<integrated_led>();
      
      if(strcmp(query.uri(), "/led/on") == 0)
      {
        led->state(true);
      }
      else if(strcmp(query.uri(), "/led/off") == 0)
      {
        led->state(false);
      }
    });

    server->start();
  }
  catch (const exception &e)
  {
    log_message(log_severity::fatal, e.what());
    log_message(log_severity::info, "Restarting in 5 seconds...");
    sleep_for(seconds(5));
    esp_restart();
  }

  log_message(log_severity::info, "Started.");
}
