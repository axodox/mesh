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

define_file(file_favicon_png, "_binary_favicon_png_start", "_binary_favicon_png_end");
define_file(file_index_html, "_binary_index_html_start", "_binary_index_html_end");
define_file(file_main_js, "_binary_main_js_start", "_binary_main_js_end");
define_file(file_polyfills_js, "_binary_polyfills_js_start", "_binary_polyfills_js_end");
define_file(file_runtime_js, "_binary_runtime_js_start", "_binary_runtime_js_end");
define_file(file_styles_css, "_binary_styles_css_start", "_binary_styles_css_end");

extern "C" void app_main()
{
  log_message(log_severity::info, "Starting...");
  dependencies.add<wifi_connection>(dependency_lifetime::singleton, []() -> unique_ptr<wifi_connection>{ return make_unique<wifi_connection>("Axodox-Ranged", "88gypARK"); } );

  try
  {
    dependencies.resolve<wifi_connection>();
    auto server = dependencies.resolve<http_server>();

    server->add_handler(http_query_method::put, "/api/led/*", [](http_query& query) {
      auto led = dependencies.resolve<integrated_led>();
      
      if(strcmp(query.uri(), "/api/led/on") == 0)
      {
        led->state(true);
      }
      else if(strcmp(query.uri(), "/api/led/off") == 0)
      {
        led->state(false);
      }
    });

    server->add_handler(http_query_method::get, "/*", [](http_query& query) {
      if(strcmp(query.uri(), "/favicon.png") == 0)
      {
        query.return_blob("image/png", file_favicon_png);
      }
      else if(strcmp(query.uri(), "/index.html") == 0 || strcmp(query.uri(), "/") == 0)
      {
        query.return_blob("text/html", file_index_html);
      }
      else if(strcmp(query.uri(), "/main.js") == 0)
      {
        query.return_blob("application/x-javascript", file_main_js);
      }
      else if(strcmp(query.uri(), "/polyfills.js") == 0)
      {
        query.return_blob("application/x-javascript", file_polyfills_js);
      }
      else if(strcmp(query.uri(), "/runtime.js") == 0)
      {
        query.return_blob("application/x-javascript", file_runtime_js);
      }
      else if(strcmp(query.uri(), "/styles.css") == 0)
      {
        query.return_blob("text/css", file_styles_css);
      }
      else
      {
        query.return_not_found();
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
