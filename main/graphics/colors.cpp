#include "colors.hpp"
#include "numerics/float3.hpp"
#include <algorithm>
#include <cmath>

using namespace mesh::numerics;

namespace mesh::graphics
{
  color_rgb::operator color_hsl() const
  {
    color_hsl result;

    auto color_rgbf = float3(r, g, b) / 255.f;

    auto max = std::max({ color_rgbf.x, color_rgbf.y, color_rgbf.z });
    auto min = std::min({ color_rgbf.x, color_rgbf.y, color_rgbf.z });

    auto diff = max - min;
    result.l = (max + min) / 2.f;
    if (std::abs(diff) < 0.00001f)
    {
      result.s = 0.f;
      result.h = 0.f;
    }
    else
    {
      if (result.l <= 0.5f)
      {
        result.s = diff / (max + min);
      }
      else
      {
        result.s = diff / (2.f - max - min);
      }

      auto dist = (float3(max) - color_rgbf) / diff;

      if (color_rgbf.x == max)
      {
        result.h = dist.z - dist.y;
      }
      else if (color_rgbf.y == max)
      {
        result.h = 2.f + dist.x - dist.z;
      }
      else
      {
        result.h = 4.f + dist.y - dist.x;
      }

      result.h = result.h * 60.f;
      if (result.h < 0.f) result.h += 360.f;
    }

    return result;
  }

  float qqh_to_rgb(float q1, float q2, float hue)
  {
    if (hue > 360.f) hue -= 360.f;
    else if (hue < 0.f) hue += 360.f;

    if (hue < 60.f) return q1 + (q2 - q1) * hue / 60.f;
    if (hue < 180.f) return q2;
    if (hue < 240.f) return q1 + (q2 - q1) * (240.f - hue) / 60.f;
    return q1;
  }

  color_hsl::operator color_rgb() const
  {
    float p2;
    if (l <= 0.5f) p2 = l * (1 + s);
    else p2 = l + s - l * s;

    float p1 = 2.f * l - p2;
    float3 color_rgbf;
    if (s == 0)
    {
      color_rgbf = float3(l);
    }
    else
    {
      color_rgbf = float3(
        qqh_to_rgb(p1, p2, h + 120.f),
        qqh_to_rgb(p1, p2, h),
        qqh_to_rgb(p1, p2, h - 120.f)
      );
    }

    color_rgbf *= 255.f;
    return color_rgb{
      (uint8_t)color_rgbf.x,
      (uint8_t)color_rgbf.y,
      (uint8_t)color_rgbf.z
    };
  }

  color_rgb lerp(const color_rgb& a, const color_rgb& b, float factor)
  {
    auto inv_factor = 1 - factor;
    return {
      uint8_t(a.r * inv_factor + b.r * factor),
      uint8_t(a.g * inv_factor + b.g * factor),
      uint8_t(a.b * inv_factor + b.b * factor)
    };
  }

  std::array<float, 256> make_gamma(float gamma, float max, float min)
  {
    std::array<float, 256> values{};
    for (auto i = 0u; i < 256; i++)
    {
      auto value = pow(i / 255.f, gamma) * 255.f * max;
      if (value < min) value = 0.f; //Ignore small values for better dithering results
      values[i] = value;
    }
    return values;
  }
}