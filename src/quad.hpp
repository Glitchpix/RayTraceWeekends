#pragma once

#include "hittable.hpp"
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
        mW{mNormal / dot(mNormal, mNormal)},
        mDistanceFromOrigin{dot(mPosition, mNormal)}, mMaterial{material} {
    setBoundingBox();
    mNormal = unitVector(mNormal);
  }

  bool hit(const Ray& ray, Interval rayRange,
           HitRecord& hitInfo) const override {
    const auto rayPlaneAngle = dot(ray.direction(), mNormal);

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

    if (unitInterval.contains(a) && unitInterval.contains(b)) {
      hitInfo.uv = {a, b};
      return true;
    }
    return false;
  }
};