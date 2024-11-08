#pragma once
#include "utils.hpp"
#include "vec3.hpp"

class Ray {
public:
  Ray() = default;

  Ray(const Vec3 origin, const Vec3 direction, double time)
      : mOrigin{origin}, mDirection{direction}, mTime{time} {};
  Ray(const Vec3 origin, const Vec3 direction)
      : mOrigin{origin}, mDirection{direction} {};

  [[nodiscard]] const Vec3& origin() const { return mOrigin; }
  [[nodiscard]] const Vec3& direction() const { return mDirection; }

  [[nodiscard]] double time() const { return mTime; }

  [[nodiscard]] Vec3 at(double t) const { return mOrigin + t * mDirection; }

  double hitSphere(const Vec3& sphereCenter, double radius) {
    Vec3 centerDirection = sphereCenter - mOrigin;
    // Quadratic solve for intersection
    const auto a = dot(mDirection, mDirection);
    const auto b = -2 * dot(mDirection, centerDirection);
    const auto c = dot(centerDirection, centerDirection) - radius * radius;
    return utils::quadraticRealSolve(a, b, c).first;
  }

private:
  Vec3 mOrigin;
  Vec3 mDirection;
  double mTime{};
};