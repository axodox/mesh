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

#include "peripherals/led_strip.hpp"
#include "peripherals/ws2812_strip.hpp"

#include "storage/spiffs.hpp"

#include "app/angular_pages.hpp"
#include "app/integrated_led_blinker.hpp"
#include "app/light_strip/light_strip_controller.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
using namespace mesh::app;
using namespace mesh::app::light_strip;
using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::peripherals;
using namespace mesh::storage;

extern "C" void app_main()
{
  log_message(log_severity::info, "Starting...");
  dependencies.add<wifi_connection>(dependency_lifetime::singleton, []() -> unique_ptr<wifi_connection> { return make_unique<wifi_connection>("Axodox-Ranged", "88gypARK"); });
  dependencies.add<led_strip>(dependency_lifetime::singleton, []() -> unique_ptr<led_strip> { return make_unique<ws2812_strip>(); });

  try
  {
    //Services
    dependencies.resolve<spiffs>();
    dependencies.resolve<wifi_connection>();
    dependencies.resolve<ntp_client>();    
    
    //App
    dependencies.resolve<angular_pages>();
    dependencies.resolve<integrated_led_blinker>();
    dependencies.resolve<light_strip_controller>();
    
    //UI
    dependencies.resolve<http_server>()->start();
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
