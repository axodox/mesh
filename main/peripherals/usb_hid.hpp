#pragma once
#include <cstdint>
#include <vector>

namespace mesh::peripherals
{
  class usb_service
  {
  public:
    static uint8_t add_string(const char* text);

    usb_service();

  private:
    static std::vector<const char*> _strings;
    bool _isInitialized = false;
  };
}