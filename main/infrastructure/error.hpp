#pragma once
#include "esp_err.h"

namespace mesh::infrastructure
{
  void check_result(const esp_err_t& error);
}