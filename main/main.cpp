#include <stdio.h>
#include <memory>
#include <exception>
#include <thread>
#include <chrono>
#include <string>

#include "nvs_flash.h"

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

  try
  {
    check_result(nvs_flash_init());
    wifi_connection connection("Axodox-Ranged", "88gypARK");
  }
  catch (const exception &e)
  {
    printf("Fatal: %s\n", e.what());
    printf("Restarting in 5 seconds...\n");
    sleep_for(seconds(5));
    esp_restart();
  }
}
