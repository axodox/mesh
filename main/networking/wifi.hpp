#pragma once
#include <memory>
#include "esp_wifi.h"

namespace mesh::networking
{
  struct esp_netif_deleter
  {
    void operator()(esp_netif_t* value);
  };

  typedef std::unique_ptr<esp_netif_t, esp_netif_deleter> esp_netif_handle_t;

  class wifi_connection
  {
  public:
    wifi_connection(const char* ssid, const char* password);
  
  private:
    esp_netif_handle_t _station;
  };
}