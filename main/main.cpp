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
#include "peripherals/ws281x_strip.hpp"

#include "storage/spiffs.hpp"

#include "app/angular_pages.hpp"
#include "app/integrated_led_blinker.hpp"
#include "app/light_strip/light_strip_controller.hpp"
#include "app/light_strip/light_strip_http_interface.hpp"

#include "app/light_strip/sources/usb_source.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
using namespace mesh::app;
using namespace mesh::app::light_strip;
using namespace mesh::app::light_strip::sources;
using namespace mesh::infrastructure;
using namespace mesh::networking;
using namespace mesh::peripherals;
using namespace mesh::storage;

extern "C" void app_main()
{
  log_message(log_severity::info, "Starting...");
  enable_usb_lamp_array();

  dependencies.add<wifi_connection>("Axodox-Home", "88gypARK");
  dependencies.add<ws281x_strip>(47, ws281x_variant::ws2815);
  dependencies.add<led_strip, ws281x_strip>();

  try
  {
    //Services
    dependencies.resolve<spiffs>();
    dependencies.resolve<wifi_connection>();
    dependencies.resolve<ntp_client>();    
    
    //App
    dependencies.resolve<angular_pages>();
    dependencies.resolve<light_strip_controller>();
    dependencies.resolve<light_strip_http_interface>();
    
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
