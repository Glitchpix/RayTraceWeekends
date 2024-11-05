#pragma once

#include "color.hpp"
#include "utils.hpp"
#include "vec3.hpp"

class Ray {
public:
  Ray() = default;

  Ray(const Vec3 origin, const Vec3 direction)
      : mOrigin{origin}, mDirection{direction} {};

  [[nodiscard]] const Vec3 &origin() const { return mOrigin; }
  [[nodiscard]] const Vec3 &direction() const { return mDirection; }

  [[nodiscard]] Vec3 at(double t) const { return mOrigin + t * mDirection; }

  double hitSphere(const Vec3 &sphereCenter, double radius) {
    Vec3 centerDirection = sphereCenter - mOrigin;
    // Quadratic solve for intersection
    const auto a = dot(mDirection, mDirection);
    const auto b = -2 * dot(mDirection, centerDirection);
    const auto c = dot(centerDirection, centerDirection) - radius * radius;
    return utils::quadraticRealSolve(a, b, c);
  }

  Color getColor() {
    const Vec3 sphereCenter = Vec3{0, 0, -1};
    const double radius = 0.5;
    const auto t = hitSphere(sphereCenter, radius);
    if (t > 0.0) {
      Vec3 normal = (this->at(t) - sphereCenter) / radius;
      Color normalColor{normal.x(), normal.y(), normal.z()};
      return utils::scaleToPositiveRange(normalColor);
    }
    Vec3 unitDirection = unit_vector(mDirection);
    Color white{1, 1, 1};
    Color blue{0, 0, 1};

    auto interpolant = utils::scaleToPositiveRange(unitDirection.y());
    return (1.0 - interpolant) * white + interpolant * blue;
  }

private:
  Vec3 mOrigin;
  Vec3 mDirection;
};