#include "wifi.hpp"

#include <cstring>
#include "infrastructure/error.hpp"
#include "infrastructure/dependencies.hpp"
#include "storage/nvs.hpp"

using namespace mesh::infrastructure;
using namespace mesh::storage;

namespace mesh::networking
{
  void esp_netif_deleter::operator()(esp_netif_t* value)
  {
    esp_netif_destroy(value);
  }

  wifi_connection::wifi_connection(const char* ssid, const char* password) :
    _event_loop(dependencies.resolve<event_loop>()),
    _on_sta_started(_event_loop->subscribe(WIFI_EVENT, WIFI_EVENT_STA_START, { this, &wifi_connection::on_sta_started })),
    _on_connected(_event_loop->subscribe(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, { this, &wifi_connection::on_connected })),
    _on_disconnected(_event_loop->subscribe(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, { this, &wifi_connection::on_disconnected }))
  {
    printf("Initializing Wi-Fi...\n");
    dependencies.resolve<nvs>();
    
    check_result(esp_netif_init());
    _station = esp_netif_handle_t(esp_netif_create_default_wifi_sta());

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    check_result(esp_wifi_init(&config));

    check_result(esp_wifi_set_storage(WIFI_STORAGE_RAM));    
    check_result(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t sta_config{};
    strcpy(reinterpret_cast<char*>(sta_config.sta.ssid), ssid);
    strcpy(reinterpret_cast<char*>(sta_config.sta.password), password);
    check_result(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));

    check_result(esp_wifi_start());
  }

  wifi_connection::~wifi_connection()
  {
    if(_is_disposing) return;    
    _is_disposing = true;

    if(_is_connected) esp_wifi_disconnect();
    esp_wifi_stop();
  }

  bool wifi_connection::is_connected() const
  {
    return _is_connected;
  }

  void wifi_connection::on_sta_started(void* arg)
  {
    printf("Connecting to Wi-Fi...\n");
    check_result(esp_wifi_connect());
  }

  void wifi_connection::on_connected(void* arg)
  {
    printf("Connected to Wi-Fi.\n");
    _is_connected = true;
  }

  void wifi_connection::on_disconnected(void* arg)
  {
    printf("Disconnected from Wi-Fi.\n");
    _is_connected = false;

    if(!_is_disposing)
    {
      check_result(esp_wifi_connect());
    }
  }
}