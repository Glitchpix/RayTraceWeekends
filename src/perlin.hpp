#pragma once
#include "intVec3.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include <cstddef>
#include <utility>

class Perlin {
public:
  Perlin() {
    for (Vec3& i : mRandomValues) {
      i = unitVector(Vec3::random(-1.0, 1.0));
    }

    generatePermutations(mPermutations);
  }

  [[nodiscard]] double noise(const Vec3& p) const {
    auto u = p.x() - std::floor(p.x());
    auto v = p.y() - std::floor(p.y());
    auto w = p.z() - std::floor(p.z());

    auto i = int(std::floor(p.x()));
    auto j = int(std::floor(p.y()));
    auto k = int(std::floor(p.z()));
    Vec3 c[2][2][2];

    for (int di = 0; di < 2; di++) {
      for (int dj = 0; dj < 2; dj++) {
        for (int dk = 0; dk < 2; dk++) {
          size_t xIndex = (i + di) & 255;
          size_t yIndex = (j + dj) & 255;
          size_t zIndex = (k + dk) & 255;
          auto finalIndex =
              size_t(mPermutations[xIndex].x ^ mPermutations[yIndex].y ^
                     mPermutations[zIndex].z);
          c[di][dj][dk] = mRandomValues[finalIndex];
        }
      }
    }

    return perlinInterpolation(c, u, v, w);
  }

  [[nodiscard]] double turbulence(const Vec3& p, int depth) const {
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
      accum += weight * noise(temp_p);
      weight *= 0.5;
      temp_p *= 2;
    }

    return std::fabs(accum);
  }

private:
  static const int kPointCount = 256;
  std::array<Vec3, kPointCount> mRandomValues;
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
  static double perlinInterpolation(const Vec3 c[2][2][2], double u, double v,
                                    double w) {
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          Vec3 weight_v(u - i, v - j, w - k);
          accum += (i * uu + (1 - i) * (1 - uu)) *
                   (j * vv + (1 - j) * (1 - vv)) *
                   (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
        }
      }
    }

    return accum;
  }
};