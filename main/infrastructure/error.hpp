#pragma once
#include "esp_err.h"
#include <exception>
#include <string>

#define check_result(error) throw_result(error, __FILE__, __LINE__);

namespace mesh::infrastructure
{
  struct esp32_error : public std::exception
  {
    esp_err_t error_code;
    std::string message;

    esp32_error(esp_err_t errorCode, const char* file = nullptr, int lineNumber = -1);
    virtual const char* what() const noexcept override;
  };

  void throw_result(const esp_err_t& error, const char* file = nullptr, int lineNumber = -1);
}