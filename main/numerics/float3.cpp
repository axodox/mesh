#include "float3.hpp"
#include <cmath>

namespace mesh::numerics
{
  float3 float3::operator*(float v)
  {
    return {
      x * v,
      y * v,
      z * v
    };
  }

  float3& float3::operator*=(float v)
  {
    *this = *this * v;
    return *this;
  }

  float3 float3::operator/(float v)
  {
    return {
      x / v,
      y / v,
      z / v
    };
  }

  float3& float3::operator/=(float v)
  {
    *this = *this / v;
    return *this;
  }

  float3 float3::operator+(const float3& other)
  {
    return {
      x + other.x,
      y + other.y,
      z + other.z
    };
  }

  float3& float3::operator+=(const float3& other)
  {
    *this = *this + other;
    return *this;
  }

  float3 float3::operator-(const float3& other)
  {
    return {
      x - other.x,
      y - other.y,
      z - other.z
    };
  }

  float3& float3::operator-=(const float3& other)
  {
    *this = *this - other;
    return *this;
  }

  float3 float3::round() const
  {
    return {
      roundf(x),
      roundf(y),
      roundf(z)
    };
  }
}