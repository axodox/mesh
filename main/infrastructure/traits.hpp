#pragma once
#include <type_traits>

namespace mesh::infrastructure
{
  template<template<typename...> class, typename...>
  struct is_instantiation_of : public std::false_type {};

  template<template<typename...> class U, typename... T>
  struct is_instantiation_of<U, U<T...>> : public std::true_type {};

  template<typename type_t>
  const char* name_of()
  {
    return type_t::type_name;
  }
}

#define nameof(type) mesh::infrastructure::name_of<type>()