#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace mesh::infrastructure
{
  template <typename item_t>
  class array_view
  {
  private:
    item_t* _start;
    item_t* _end;

  public:
    typedef item_t value_type;

    array_view() :
      _start(nullptr),
      _end(nullptr)
    { }

    array_view(std::vector<item_t>& items) :
      _start(items.data()),
      _end(items.data() + items.size())
    { }

    array_view(const std::vector<item_t>& items) :
      _start(const_cast<item_t*>(items.data())),
      _end(const_cast<item_t*>(items.data() + items.size()))
    { }

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
      return _end;
    }

    const item_t* begin() const
    {
      return _start;
    }

    const item_t* end() const
    {
      return _end;
    }

    item_t* data()
    {
      return _start;
    }

    const item_t* data() const
    {
      return _start;
    }

    size_t size() const
    { 
      return _end - _start; 
    }
  };
}