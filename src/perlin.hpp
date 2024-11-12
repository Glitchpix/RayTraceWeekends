#pragma once
#include "intVec3.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include <cstddef>
#include <utility>

class Perlin {
public:
  Perlin() {
    for (double& i : mRandomValues) {
      i = utils::randomDouble();
    }

    generatePermutations(mPermutations);
  }

  [[nodiscard]] double noise(const Vec3& p) const {
    auto i = int(4 * p.x()) & 255;
    auto j = int(4 * p.y()) & 255;
    auto k = int(4 * p.z()) & 255;

    auto index = mPermutations[size_t(i)].x ^ mPermutations[size_t(j)].y ^
                 mPermutations[size_t(k)].z;

    return mRandomValues[size_t(index)];
  }

private:
  static const int kPointCount = 256;
  std::array<double, kPointCount> mRandomValues;
  std::array<TVec3<int>, kPointCount> mPermutations;

  static void
  generatePermutations(std::array<TVec3<int>, kPointCount>& points) {
    for (int i = 0; i < kPointCount; i++) {
      points[size_t(i)] = TVec3<int>{i, i, i};
    }

    permute(points, kPointCount);
  }

  static void permute(std::array<TVec3<int>, kPointCount>& points, size_t n) {
    for (size_t i = n - 1; i > 0; i--) {
      auto target = static_cast<size_t>(utils::randomInt(0, int(i)));
      std::swap(points[i], points[target]);
    }
  }
};