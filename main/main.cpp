#include <stdio.h>
#include <memory>
#include <exception>
#include <thread>
#include <chrono>
#include <string>

#include "infrastructure/bitwise_operations.hpp"
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

define_file(file_favicon_png, "_binary_favicon_png_gz_start", "_binary_favicon_png_gz_end");
define_file(file_index_html, "_binary_index_html_gz_start", "_binary_index_html_gz_end");
define_file(file_main_js, "_binary_main_js_gz_start", "_binary_main_js_gz_end");
define_file(file_polyfills_js, "_binary_polyfills_js_gz_start", "_binary_polyfills_js_gz_end");
define_file(file_runtime_js, "_binary_runtime_js_gz_start", "_binary_runtime_js_gz_end");
define_file(file_styles_css, "_binary_styles_css_gz_start", "_binary_styles_css_gz_end");

extern "C" void app_main()
{
  log_message(log_severity::info, "Starting...");
  dependencies.add<wifi_connection>(dependency_lifetime::singleton, []() -> unique_ptr<wifi_connection> { return make_unique<wifi_connection>("Axodox-Ranged", "88gypARK"); });

  try
  {
    dependencies.resolve<wifi_connection>();
    auto server = dependencies.resolve<http_server>();

    server->add_handler(http_query_method::put, "/api/led/*", [](http_query &query)
    {
      auto led = dependencies.resolve<integrated_led>();

      if (strcmp(query.uri(), "/api/led/on") == 0)
      {
        led->state(true);
      }
      else if (strcmp(query.uri(), "/api/led/off") == 0)
      {
        led->state(false);
      }
    });

    auto static_file_options = http_static_file_options::compress_gzip;
    server->add_static_file("/favicon.png", mime_type::png, file_favicon_png, static_file_options);
    server->add_static_file("/index.html", mime_type::html, file_index_html, bitwise_or(static_file_options, http_static_file_options::redirect_root));
    server->add_static_file("/main.js", mime_type::js, file_main_js, static_file_options);
    server->add_static_file("/polyfills.js", mime_type::js, file_polyfills_js, static_file_options);
    server->add_static_file("/runtime.js", mime_type::js, file_runtime_js, static_file_options);
    server->add_static_file("/styles.css", mime_type::css, file_styles_css, static_file_options);

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
