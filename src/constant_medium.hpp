#pragma once

#include "hittable.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include <cmath>
#include <memory>
class ConstantMedium : public Hittable {
public:
  ConstantMedium(double density, std::shared_ptr<Texture> texture,
                 std::shared_ptr<Hittable> boundry)
      : mNegativeInverseDensity{-1.0 / density},
        phaseMaterial{std::make_shared<Isotropic>(texture)},
        mBoundary{boundry} {}
  ConstantMedium(double density, const Color& albedo,
                 std::shared_ptr<Hittable> boundry)
      : mNegativeInverseDensity{-1.0 / density},
        phaseMaterial{std::make_shared<Isotropic>(albedo)}, mBoundary{boundry} {
  }

  bool hit(const Ray& ray, Interval rayRange,
           HitRecord& hitInfo) const override {
    HitRecord entryPoint;
    HitRecord exitPoint;

    if (!mBoundary->hit(ray, Interval::universe, entryPoint)) {
      return false;
    }

    if (!mBoundary->hit(
            ray, Interval{entryPoint.t + kEpsilon, utils::INFINITE_DOUBLE},
            exitPoint)) {
      return false;
    }

    entryPoint.t = std::fmax(entryPoint.t, rayRange.min());
    exitPoint.t = std::fmin(exitPoint.t, rayRange.max());

    if (entryPoint.t >= exitPoint.t) {
      return false;
    }

    if (entryPoint.t < 0) {
      entryPoint.t = 0;
    }

    auto rayLength = ray.direction().length();
    auto distanceInsideVolume = (exitPoint.t - entryPoint.t) * rayLength;
    auto hitDistance =
        mNegativeInverseDensity * std::log(utils::randomDouble());

    if (hitDistance > distanceInsideVolume) {
      return false;
    }

    hitInfo.t = entryPoint.t + hitDistance / rayLength;
    hitInfo.position = ray.at(hitInfo.t);

    hitInfo.setFaceNormal(ray, Vec3{1, 0, 0}); // arbitrary
    hitInfo.material = phaseMaterial;
    hitInfo.uv = {utils::randomDouble(), utils::randomDouble()};
    return true;
  }

  [[nodiscard]] AABB boundingBox() const override {
    return mBoundary->boundingBox();
  }

private:
  static constexpr double kEpsilon = 0.0001;
  double mNegativeInverseDensity;
  std::shared_ptr<IMaterial> phaseMaterial;
  std::shared_ptr<Hittable> mBoundary;
};