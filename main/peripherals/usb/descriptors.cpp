#include "descriptors.hpp"

namespace mesh::peripherals::usb
{
  constexpr uint8_t endpoint_address(uint8_t lane, endpoint_direction direction)
  {
    return (lane & 0xFF) | uint8_t(direction);
  }
}
