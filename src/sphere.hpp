#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class Sphere : public Hittable {
public:
  Sphere(const Vec3 &center, double radius)
      : mCenter{center}, mRadius{std::fmax(0, radius)} {};

  bool hit(const Ray &ray, double tMin, double tMax,
           HitRecord &hitInfo) const override {
    Vec3 centerDirection = mCenter - ray.origin();
    // Quadratic solve for intersection
    const auto a = dot(ray.direction(), ray.direction());
    const auto b = -2 * dot(ray.direction(), centerDirection);
    const auto c = dot(centerDirection, centerDirection) - mRadius * mRadius;
    const auto [negRoot, posRoot] = utils::quadraticRealSolve(a, b, c);
    if (negRoot < 0) {
      return false;
    }

    double hitRoot = negRoot;
    if (negRoot <= tMin || negRoot >= tMax) {
      hitRoot = posRoot;
      if (posRoot <= tMin || posRoot >= tMax) {
        return false;
      }
    }

    hitInfo.t = hitRoot;
    hitInfo.position = ray.at(hitRoot);
    Vec3 normal = (hitInfo.position - mCenter) / mRadius;
    hitInfo.setFaceNormal(ray, normal);
    return true;
  }

private:
  Vec3 mCenter;
  double mRadius;
};