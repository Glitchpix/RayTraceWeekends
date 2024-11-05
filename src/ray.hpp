#pragma once

#include "color.hpp"
#include "vec3.hpp"

class Ray {
public:
  Ray() = default;

  Ray(const Vec3 origin, const Vec3 direction)
      : mOrigin{origin}, mDirection{direction} {};

  [[nodiscard]] const Vec3 &origin() const { return mOrigin; }
  [[nodiscard]] const Vec3 &direction() const { return mDirection; }

  [[nodiscard]] Vec3 at(double t) const { return mOrigin + t * mDirection; }

  Color getColor() {
    Vec3 unitDirection = unit_vector(mDirection);
    Color white{1, 1, 1};
    Color blue{0, 0, 1};
    constexpr double scaleFactor = 0.5; // [-1,1] -> [0,1]
    auto interpolant = scaleFactor * (unitDirection.y() + 1.0);
    return (1.0 - interpolant) * white + interpolant * blue;
  }

private:
  Vec3 mOrigin;
  Vec3 mDirection;
};