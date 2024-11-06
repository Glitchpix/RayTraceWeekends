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

  constexpr double maxColorValue = 255.0;

  // Translate the [0,1] component values to the byte range [0,255].
  constexpr Interval intensity{0.0, 0.99999};
  int rbyte = static_cast<int>(round(maxColorValue * intensity.clamp(r)));
  int gbyte = static_cast<int>(round(maxColorValue * intensity.clamp(g)));
  int bbyte = static_cast<int>(round(maxColorValue * intensity.clamp(b)));

  // Write out the pixel color components.
  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
} // namespace color
