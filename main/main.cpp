#include <stdio.h>
#include <memory>
#include <exception>
#include <thread>
#include <chrono>
#include <string>

#include "infrastructure/dependencies.hpp"
#include "infrastructure/error.hpp"
#include "networking/wifi.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
using namespace mesh::infrastructure;
using namespace mesh::networking;

extern "C" void app_main()
{
  printf("Hello world!\n");
  dependencies.add<wifi_connection>(dependency_lifetime::singleton, []() -> unique_ptr<wifi_connection>{ return make_unique<wifi_connection>("Axodox-Ranged", "88gypARK"); } );

  try
  {
    dependencies.resolve<wifi_connection>();
  }
  catch (const exception &e)
  {
    printf("Fatal: %s\n", e.what());
    printf("Restarting in 5 seconds...\n");
    sleep_for(seconds(5));
    esp_restart();
  }
}
