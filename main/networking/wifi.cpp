#include "wifi.hpp"

#include <cstring>
#include "infrastructure/error.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  void esp_netif_deleter::operator()(esp_netif_t* value)
  {
    esp_netif_destroy(value);
  }

  wifi_connection::wifi_connection(const char* ssid, const char* password)
  {
    check_result(esp_netif_init());
    _station = esp_netif_handle_t(esp_netif_create_default_wifi_sta());

    wifi_init_config_t config WIFI_INIT_CONFIG_DEFAULT()
    check_result(esp_wifi_init(&config));
    check_result(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t staConfig{};
    strcpy(reinterpret_cast<char*>(staConfig.sta.ssid), ssid);
    strcpy(reinterpret_cast<char*>(staConfig.sta.password), password);
    check_result(esp_wifi_set_config(ESP_IF_WIFI_STA, &staConfig));

    check_result(esp_wifi_start());
    check_result(esp_wifi_connect());
  }
}