#include "hasher.hpp"

namespace mesh::infrastructure
{
  constexpr size_t fnv_offset_basis()
  {
    if constexpr (sizeof(size_t) == 8)
    {
      return 14695981039346656037ULL;
    }
    else
    {
      return 2166136261U;
    }
  }

  constexpr size_t fnv_prime()
  {
    if constexpr (sizeof(size_t) == 8)
    {
      return 2166136261U;
    }
    else
    {
      return 16777619U;
    }
  }

  void fnv_append_bytes(size_t &hash, const unsigned char *const data, const size_t length) noexcept
  {
    for (size_t i = 0; i < length; ++i)
    {
      hash ^= static_cast<size_t>(data[i]);
      hash *= fnv_prime();
    }
  }

  size_t hash_mem(const unsigned char *const data, const size_t length)
  {
    size_t hash = fnv_offset_basis();
    fnv_append_bytes(hash, data, length);
    return hash;
  }
}