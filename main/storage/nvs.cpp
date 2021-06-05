#include "nvs.hpp"

#include "infrastructure/error.hpp"
#include "nvs_flash.h"

using namespace mesh::infrastructure;

namespace mesh::storage
{
  nvs::nvs()
  {
    auto result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
      check_result(nvs_flash_erase());
      check_result(nvs_flash_init());
    }
    else
    {
      check_result(result);
    }
  }
}