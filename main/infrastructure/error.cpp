#include "error.hpp"

namespace mesh::infrastructure
{
  void check_result(const esp_err_t& error)
  {
    ESP_ERROR_CHECK(error);
  }
}