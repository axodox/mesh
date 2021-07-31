#pragma once
#include "lwip/sockets.h"

namespace mesh::networking
{
  class socket_handle
  {
  private:
    int _value;

  public:
    socket_handle();
    socket_handle(int value);

    const int& get() const;
    void reset();
    void operator=(int value);
    ~socket_handle();
  };
}