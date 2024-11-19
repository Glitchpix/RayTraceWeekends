#pragma once

#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include <memory>

class Quad : public Hittable {
public:
  Quad(const Vec3& position, const Vec3& widthVector, const Vec3& heightVector,
       std::shared_ptr<IMaterial> material)
      : mPosition{position}, mWidthVector{widthVector},
        mHeightVector{heightVector},
        mNormal{cross(mWidthVector, mHeightVector)},
        mW{mNormal / dot(mNormal, mNormal)}, mMaterial{material} {
    setBoundingBox();
    mNormal = unitVector(mNormal);
    mDistanceFromOrigin = {dot(mNormal, mPosition)};
  }

  bool hit(const Ray& ray, Interval rayRange,
           HitRecord& hitInfo) const override {
    const auto rayPlaneAngle = dot(mNormal, ray.direction());

    if (std::fabs(rayPlaneAngle) < kEpsilon) {
      return false;
    }

    const auto t =
        (mDistanceFromOrigin - dot(mNormal, ray.origin())) / rayPlaneAngle;

    if (!rayRange.contains(t)) {
      return false;
    }

    const auto intersection = ray.at(t);

    Vec3 planeHitVector = intersection - mPosition;
    auto alpha = dot(mW, cross(planeHitVector, mHeightVector));
    auto beta = dot(mW, cross(mWidthVector, planeHitVector));

    if (!isInterior(alpha, beta, hitInfo)) {
      return false;
    }

    hitInfo.t = t;
    hitInfo.position = intersection;
    hitInfo.material = mMaterial;
    hitInfo.setFaceNormal(ray, mNormal);

    return true;
  }

  [[nodiscard]] AABB boundingBox() const override { return mBoundingBox; }

private:
  Vec3 mPosition;
  Vec3 mWidthVector;
  Vec3 mHeightVector;
  Vec3 mNormal;
  Vec3 mW;
  double mDistanceFromOrigin{0};
  std::shared_ptr<IMaterial> mMaterial;
  AABB mBoundingBox;

  static constexpr double kEpsilon{1e-8};

  void setBoundingBox() {
    const auto firstDiagonal =
        AABB{mPosition, mPosition + mWidthVector + mHeightVector};
    const auto secondDiagonal =
        AABB{mPosition + mWidthVector, mPosition + mHeightVector};
    mBoundingBox = {firstDiagonal, secondDiagonal};
  }

  static bool isInterior(double a, double b, HitRecord& hitInfo) {
    const auto unitInterval = Interval(0, 1);

    if (!unitInterval.contains(a) || !unitInterval.contains(b)) {
      return false;
    }
    hitInfo.uv = {a, b};
    return true;
  }
};

inline std::shared_ptr<HittableList> box(const Vec3& a, const Vec3& b,
                                         std::shared_ptr<IMaterial> material) {
  // Returns the 3D box (six sides) that contains the two opposite vertices a &
  // b.

  auto sides = std::make_shared<HittableList>();

  // Construct the two opposite vertices with the minimum and maximum
  // coordinates.
  auto min = Vec3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()),
                  std::fmin(a.z(), b.z()));
  auto max = Vec3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()),
                  std::fmax(a.z(), b.z()));

  auto dx = Vec3(max.x() - min.x(), 0, 0);
  auto dy = Vec3(0, max.y() - min.y(), 0);
  auto dz = Vec3(0, 0, max.z() - min.z());

  sides->add(make_shared<Quad>(Vec3(min.x(), min.y(), max.z()), dx, dy,
                               material)); // front
  sides->add(make_shared<Quad>(Vec3(max.x(), min.y(), max.z()), -dz, dy,
                               material)); // right
  sides->add(make_shared<Quad>(Vec3(max.x(), min.y(), min.z()), -dx, dy,
                               material)); // back
  sides->add(make_shared<Quad>(Vec3(min.x(), min.y(), min.z()), dz, dy,
                               material)); // left
  sides->add(make_shared<Quad>(Vec3(min.x(), max.y(), max.z()), dx, -dz,
                               material)); // top
  sides->add(make_shared<Quad>(Vec3(min.x(), min.y(), min.z()), dx, dz,
                               material)); // bottom

  return sides;
}