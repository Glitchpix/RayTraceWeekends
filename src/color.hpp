#pragma once
#include "interval.hpp"
#include "vec3.hpp"

using Color = Vec3;

namespace color {

inline double linearToGamma(double linearComponent) {
  if (linearComponent > 0) {
    return std::sqrt(linearComponent);
  }

  return 0;
}
void write(std::ostream& out, const Color& pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  r = linearToGamma(r);
  g = linearToGamma(g);
  b = linearToGamma(b);

  constexpr int maxColorValue = 256;

  // Translate the [0,1] component values to the byte range [0,255].
  constexpr Interval intensity{0.0, 0.99999};
  int rbyte = int(maxColorValue * intensity.clamp(r));
  int gbyte = int(maxColorValue * intensity.clamp(g));
  int bbyte = int(maxColorValue * intensity.clamp(b));

  // Write out the pixel color components.
  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

static constexpr Color Black{0, 0, 0};
static constexpr Color White{1, 1, 1};
static constexpr Color Red{1, 0, 0};
static constexpr Color Green{0, 1, 0};
static constexpr Color Blue{0, 0, 1};

static constexpr Color mDebugColor = Color{0, 1, 1};

static constexpr int maxIntegerColorValue = 255;
static constexpr float maxIntegerFloatValue = 255.0;
} // namespace color
