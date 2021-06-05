#include "error.hpp"

using namespace std;

namespace mesh::infrastructure
{
  esp32_error::esp32_error(esp_err_t errorCode, const char* file, int lineNumber) :
    error_code(errorCode)
  { 
    message = "Exception encountered in " + string(file) + " on line " + to_string(lineNumber);

#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
    message += ": " + string(esp_err_to_name(error_code));
#else
    message += ".";
#endif //CONFIG_ESP_ERR_TO_NAME_LOOKUP
  }

  const char* esp32_error::what() const noexcept
  {
    return message.c_str();
  }

  void throw_result(const esp_err_t& error, const char* file, int lineNumber)
  {
    if(error != ESP_OK) throw esp32_error(error, file, lineNumber);
  }
}