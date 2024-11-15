#pragma once

#include "hittable.hpp"
#include "material.hpp"
#include <memory>

class Quad : public Hittable {
public:
  Quad(const Vec3& position, const Vec3& widthVector, const Vec3& heightVector,
       std::shared_ptr<IMaterial> material)
      : mPosition{position}, mWidthVector{widthVector},
        mHeightVector{heightVector}, mMaterial{material} {
    setBoundingBox();
  }

  bool hit(const Ray& ray, Interval rayRange,
           HitRecord& hitInfo) const override {
    (void)ray;
    (void)rayRange;
    (void)hitInfo;
    // TODO
    return false;
  }

  [[nodiscard]] AABB boundingBox() const override { return mBoundingBox; }

private:
  Vec3 mPosition;
  Vec3 mWidthVector;
  Vec3 mHeightVector;
  std::shared_ptr<IMaterial> mMaterial;
  AABB mBoundingBox;

  void setBoundingBox() {
    const auto firstDiagonal =
        AABB{mPosition, mPosition + mWidthVector + mHeightVector};
    const auto secondDiagonal =
        AABB{mPosition + mWidthVector, mPosition + mHeightVector};
    mBoundingBox = {firstDiagonal, secondDiagonal};
  }
};