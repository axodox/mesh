#include "wifi.hpp"
#include "infrastructure/error.hpp"

using namespace mesh::infrastructure;

namespace mesh::networking
{
  void esp_netif_deleter::operator()(esp_netif_t* value)
  {
    esp_netif_destroy(value);
  }

  wifi_connection::wifi_connection()
  {
    check_result(esp_netif_init());
    _station = esp_netif_handle_t(esp_netif_create_default_wifi_sta());
  }
}