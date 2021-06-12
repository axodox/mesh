#pragma once
#include <cstddef>
#include <cstdint>

namespace mesh::infrastructure
{
  template <typename item_t>
  class array_view
  {
  private:
    item_t* _start;
    item_t* _end;

  public:
    constexpr array_view(item_t* start, item_t* end) :
      _start(start),
      _end(end)
    { }

    constexpr array_view(const item_t* start, const item_t* end) :
      _start(const_cast<item_t*>(start)),
      _end(const_cast<item_t*>(end))
    { }

    item_t* begin()
    {
      return _start;
    }

    item_t* end()
    {
      return _start;
    }

    const item_t* begin() const
    {
      return _start;
    }

    const item_t* end() const
    {
      return _start;
    }

    size_t size() const
    { 
      return _end - _start; 
    }
  };
}