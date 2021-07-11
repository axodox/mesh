#include <memory>
#include <exception>
#include <thread>
#include <chrono>

#include "infrastructure/dependencies.hpp"
#include "infrastructure/error.hpp"
#include "infrastructure/logger.hpp"
#include "networking/http_server.hpp"
#include "networking/wifi.hpp"
#include "networking/ntp_client.hpp"

#include "peripherals/ws2812_strip.hpp" //

#include "app/angular_pages.hpp"
#include "app/integrated_led_blinker.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
using namespace mesh::app;
using namespace mesh::infrastructure;
using namespace mesh::networking;

using namespace mesh::peripherals; //
using namespace mesh::graphics; //

extern "C" void app_main()
{
  log_message(log_severity::info, "Starting...");
  dependencies.add<wifi_connection>(dependency_lifetime::singleton, []() -> unique_ptr<wifi_connection> { return make_unique<wifi_connection>("Axodox-Ranged", "88gypARK"); });

  try
  {
    //Services
    dependencies.resolve<wifi_connection>();
    dependencies.resolve<ntp_client>();
    
    //App
    dependencies.resolve<angular_pages>();
    dependencies.resolve<integrated_led_blinker>();
    
    //UI
    dependencies.resolve<http_server>()->start();

    auto leds = dependencies.resolve<ws2812_strip>();
    vector<color_rgb> colors = {
      {10, 0, 0},
      {0, 10, 0},
      {0, 0, 10},
      {10, 10, 10}
    };

    leds->push_pixels(colors);
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
