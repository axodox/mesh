#include "stream.hpp"
#include <stdexcept>

using namespace std;

namespace mesh::storage
{
  size_t stream::position() const
  {
    throw logic_error("This method is not supported.");
  }

  void stream::seek(size_t position)
  {
    throw logic_error("This method is not supported.");
  }

  size_t stream::length() const
  {
    throw logic_error("This method is not supported.");
  }

  void stream::flush() 
  { }
}