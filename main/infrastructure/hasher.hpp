#pragma once
#include <type_traits>
#include <functional>
#include <cstring>

namespace mesh::infrastructure
{
  size_t hash_mem(const unsigned char *const data, const size_t length);

  template<typename value_t>
  size_t hash_value(const value_t& value)
  {
    static_assert(std::is_trivial_v<value_t>, "Only trivial types can be directly hashed.");
    return hash_mem(reinterpret_cast<const unsigned char*>(&value), sizeof(value_t));
  }

  template <typename value_t> 
  struct trivial_hasher
  {
    size_t operator()(const value_t& value) const
    {
      static_assert(std::is_trivial_v<value_t>, "The type must be trivial for trivial hasher.");
      if constexpr (sizeof(value_t) <= sizeof(size_t)) //If the value type is smaller than size_t, return the value itself (with padding if needed)
      {
        size_t result = {};
        memcpy(&result, &value, sizeof(value_t));
        return result;
      }
      else //Otherwise call efficient MSVC FNV hash implementation
      {
        return hash_value(value);
      }
    }
  };

  template <typename value_t>
  struct trivial_comparer
  {
    bool operator()(const value_t& a, const value_t& b) const
    {
      static_assert(std::is_trivial_v<value_t>, "The type must be trivial for trivial comparer.");
      return memcmp(&a, &b, sizeof(value_t)) == 0;
    }
  };
}