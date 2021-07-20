#pragma once

namespace mesh::numerics
{
  struct float3
  {
    float x, y, z;

    float3() = default;
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
  };
}