#pragma once

namespace mesh::numerics
{
  struct float3
  {
    float x, y, z;

    constexpr float3() :
      x(0),
      y(0),
      z(0)
    { }

    constexpr float3(float v) :
      x(v),
      y(v),
      z(v)
    { }

    constexpr float3(float x, float y, float z) :
      x(x),
      y(y),
      z(z)
    { }

    float3 operator*(float v);
    float3& operator*=(float v);

    float3 operator/(float v);
    float3& operator/=(float v);

    float3 operator+(const float3& other);
    float3& operator+=(const float3& other);

    float3 operator-(const float3& other);
    float3& operator-=(const float3& other);

    float3 round() const;

    bool operator==(const float3&) const = default;
    bool operator!=(const float3&) const = default;
  };
}