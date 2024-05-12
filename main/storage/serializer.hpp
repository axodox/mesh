#pragma once
#include <type_traits>

#include "stream.hpp"
#include "infrastructure/traits.hpp"
#include "infrastructure/array_view.hpp"

namespace mesh::storage
{
  template<typename T>
  struct serializer<T, typename std::enable_if_t<std::conjunction_v<std::is_trivially_copyable<T>, std::negation<infrastructure::is_instantiation_of<infrastructure::array_view, T>>>>>
  {
    static void serialize(stream& stream, const T& value)
    {
      stream.write({ reinterpret_cast<const uint8_t*>(&value), sizeof(T) });
    }

    static void deserialize(stream& stream, T& value)
    {
      stream.read({ reinterpret_cast<uint8_t*>(&value), sizeof(T) });
    }
  };

  template<typename T>
  struct serializer<T, typename std::enable_if_t<infrastructure::is_instantiation_of<infrastructure::array_view, T>::value>>
  {
    static void serialize(stream& stream, const T& value)
    {
      if constexpr(std::is_trivially_copyable<typename T::value_type>::value)
      {
        stream.write(value.size() * sizeof(typename T::value_type), reinterpret_cast<const uint8_t*>(value.data()));
      }
      else
      {
        for (auto& item : value)
        {
          stream.write(item);
        }
      }
    }

    static void deserialize(stream& stream, T& value)
    {
      if constexpr(std::is_trivially_copyable<typename T::value_type>::value)
      {
        stream.read({ reinterpret_cast<uint8_t*>(value.data()), value.size() * sizeof(typename T::value_type) });
      }
      else
      {
        for (auto& item : value)
        {
          stream.read(item);
        }
      }
    }
  };
}