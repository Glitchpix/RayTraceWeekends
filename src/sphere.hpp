#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include <memory>

class Sphere : public Hittable {
public:
  Sphere(const Vec3& center, double radius, std::shared_ptr<IMaterial> material)
      : mCenter{center, {0, 0, 0}}, mRadius{std::fmax(0, radius)},
        mMaterial{material} {
    const auto radiusVector = Vec3{mRadius, mRadius, mRadius};
    mBoundingBox = {center - radiusVector, center + radiusVector};
  };

  Sphere(const Vec3& startCenter, const Vec3& endCenter, double radius,
         std::shared_ptr<IMaterial> material)
      : mCenter{startCenter, {endCenter - startCenter}},
        mRadius{std::fmax(0, radius)}, mMaterial{material} {
    const auto radiusVector = Vec3{mRadius, mRadius, mRadius};
    AABB startBox{mCenter.at(0) - radiusVector, mCenter.at(0) + radiusVector};
    AABB endBox{mCenter.at(1) - radiusVector, mCenter.at(1) + radiusVector};
    mBoundingBox = AABB{startBox, endBox};
  };

  bool hit(const Ray& ray, Interval rayRange,
           HitRecord& hitInfo) const override {

    // TODO: Find out why normal method produced weird visual bug?
    const Vec3 currentCenter = mCenter.at(ray.time());
    Vec3 oc = currentCenter - ray.origin();
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
    Vec3 normal = (hitInfo.position - currentCenter) / mRadius;
    hitInfo.setFaceNormal(ray, normal);
    hitInfo.uv = getSphereUV(normal);
    return true;
  }

  [[nodiscard]] AABB boundingBox() const override { return mBoundingBox; }

private:
  Ray mCenter;
  double mRadius;
  std::shared_ptr<IMaterial> mMaterial;
  AABB mBoundingBox;

  static Vec2 getSphereUV(const Vec3& point) {
    // point: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    auto theta = std::acos(-point.y());
    auto phi = std::atan2(-point.z(), point.x()) + utils::PI;

    double u = phi / (2 * utils::PI);
    double v = theta / utils::PI;
    return {u, v};
  }
};