#include "socket_handle.hpp"

namespace mesh::networking
{
  socket_handle::socket_handle() :
    _value(0)
  { }

  socket_handle::socket_handle(int value) :
    _value(value)
  { }

  const int& socket_handle::get() const
  {
    return _value;
  }

  void socket_handle::reset()
  {
    if(_value)
    {
      closesocket(_value);
      _value = 0;
    }
  }

  void socket_handle::operator=(int value)
  {
    reset();
    _value = value;
  }

  socket_handle::~socket_handle()
  {
    reset();
  }
}