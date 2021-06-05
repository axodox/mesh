#pragma once
#include <memory>

#include "esp_wifi.h"
#include "infrastructure/event_loop.hpp"

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
    ~wifi_connection();
    bool is_connected() const;
  
  private:
    esp_netif_handle_t _station;
    std::shared_ptr<mesh::infrastructure::event_loop> _event_loop;
    mesh::infrastructure::event_loop_subscription _on_sta_started, _on_connected, _on_disconnected;
    bool _is_connected = false;
    bool _is_disposing = false;

    void on_sta_started(void* arg);
    void on_connected(void* arg);
    void on_disconnected(void* arg);
  };
}