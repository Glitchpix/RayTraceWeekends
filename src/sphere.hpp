#pragma once

#include "hittable.hpp"
#include "vec3.hpp"
#include <memory>

class Sphere : public Hittable {
public:
  Sphere(const Vec3& center, double radius, std::shared_ptr<IMaterial> material)
      : mCenter{center}, mRadius{std::fmax(0, radius)}, mMaterial{material} {};

  bool hit(const Ray& ray, Interval rayRange,
           HitRecord& hitInfo) const override {

    // TODO: Find out why normal method produced weird visual bug?
    Vec3 oc = mCenter - ray.origin();
    auto a = ray.direction().length_squared();
    auto h = dot(ray.direction(), oc);
    auto c = oc.length_squared() - mRadius * mRadius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0) {
      return false;
    }

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!rayRange.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!rayRange.surrounds(root)) {
        return false;
      }
    }

    hitInfo.t = root;
    hitInfo.position = ray.at(root);
    hitInfo.material = mMaterial;
    Vec3 normal = (hitInfo.position - mCenter) / mRadius;
    hitInfo.setFaceNormal(ray, normal);
    return true;
  }

private:
  Vec3 mCenter;
  double mRadius;
  std::shared_ptr<IMaterial> mMaterial;
};