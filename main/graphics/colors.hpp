#pragma once
#include <array>
#include <cstdint>

namespace mesh::graphics
{
  struct color_hsl;

  struct color_rgb
  {
    uint8_t g, r, b;

    color_rgb() = default;
    constexpr color_rgb(uint8_t r, uint8_t g, uint8_t b) :
      g(g),
      r(r),
      b(b)
    { }

    operator color_hsl() const;
  };

  color_rgb lerp(const color_rgb& a, const color_rgb& b, float factor);

  struct color_hsl
  {
    float h, s, l; //h: [0..360], s: [0..1], l: [0..1]

    color_hsl() = default;
    constexpr color_hsl(float h, float s, float l) :
      h(h),
      s(s),
      l(l)
    { }

    operator color_rgb() const;
  };

  std::array<float, 256> make_gamma(float gamma, float max = 1.f, float min = 0.1f);
}