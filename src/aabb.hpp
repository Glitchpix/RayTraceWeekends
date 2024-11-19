#pragma once

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"
struct AxisAlignedBoundingBox {
  Interval mX, mY, mZ;

  AxisAlignedBoundingBox() = default;

  AxisAlignedBoundingBox(const Interval& x, const Interval& y,
                         const Interval& z)
      : mX{x}, mY{y}, mZ{z} {
    padToMinimums();
  };

  AxisAlignedBoundingBox(const Vec3& extremaA, const Vec3& extremaB) {
    mX = Interval(std::fmin(extremaA[0], extremaB[0]),
                  std::fmax(extremaA[0], extremaB[0]));
    mY = Interval(std::fmin(extremaA[1], extremaB[1]),
                  std::fmax(extremaA[1], extremaB[1]));
    mZ = Interval(std::fmin(extremaA[2], extremaB[2]),
                  std::fmax(extremaA[2], extremaB[2]));
    padToMinimums();
  };

  AxisAlignedBoundingBox(const AxisAlignedBoundingBox& first,
                         const AxisAlignedBoundingBox& second) {
    mX = Interval{first.mX, second.mX};
    mY = Interval{first.mY, second.mY};
    mZ = Interval{first.mZ, second.mZ};
  }

  [[nodiscard]] const Interval& axisInterval(size_t axisIndex) const {
    if (axisIndex == 1) {
      return mY;
    }
    if (axisIndex == 2) {
      return mZ;
    }
    return mX;
  }

  [[nodiscard]] bool hit(const Ray& incoming, Interval rayT) const {
    const Vec3& rayOrigin = incoming.origin();
    const Vec3& rayDirection = incoming.direction();

    auto lowerT = rayT.min();
    auto upperT = rayT.max();
    for (size_t iAxis = 0; iAxis < 3; ++iAxis) {
      const Interval& currentAxis = axisInterval(iAxis);
      const double axisDirectionInverse = 1.0 / rayDirection[iAxis];
      auto t0 = (currentAxis.min() - rayOrigin[iAxis]) * axisDirectionInverse;
      auto t1 = (currentAxis.max() - rayOrigin[iAxis]) * axisDirectionInverse;

      if (t0 > t1) {
        std::swap(t0, t1);
      }

      if (t0 > lowerT) {
        lowerT = t0;
      }
      if (t1 < upperT) {
        upperT = t1;
      }

      if (upperT <= lowerT) {
        return false;
      }
    }
    return true;
  }

  [[nodiscard]] size_t longestAxis() const {
    if (mX.size() > mY.size()) {
      return mX.size() > mZ.size() ? 0 : 2;
    }
    return mY.size() > mZ.size() ? 1 : 2;
  }
  static const AxisAlignedBoundingBox empty, universe;

private:
  static constexpr double kMinimumPadding = 0.0001;
  void padToMinimums() {

    auto constexpr expandFunc = [](Interval& interval) {
      if (interval.size() < kMinimumPadding) {
        interval = interval.expand(kMinimumPadding);
      }
    };
    expandFunc(mX);
    expandFunc(mY);
    expandFunc(mZ);
  }
};

using AABB = AxisAlignedBoundingBox;

const AABB AABB::empty =
    AABB{Interval::empty, Interval::empty, Interval::empty};
const AABB AABB::universe =
    AABB{Interval::universe, Interval::universe, Interval::universe};

AABB operator+(const AABB& box, const Vec3& offset) {
  return AABB{box.mX + offset.x(), box.mY + offset.y(), box.mZ + offset.z()};
}

AABB operator+(const Vec3& offset, const AABB& box) { return box + offset; }