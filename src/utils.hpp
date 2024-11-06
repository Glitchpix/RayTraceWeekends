#pragma once

#include <cmath>
#include <limits>
#include <numbers>
#include <random>
#include <utility>

namespace utils {
constexpr double INFINITE_DOUBLE = std::numeric_limits<double>::infinity();
constexpr double PI = std::numbers::pi;

inline double toRadians(double degrees) { return degrees * PI / 180.0; }

inline std::pair<double, double> quadraticRealSolve(double a, double b,
                                                    double c) {
  const double discriminant = b * b - 4 * a * c;
  if (discriminant < 0.0) {
    return {-1.0, -1.0};
  }
  const double negativeNum = (-b - std::sqrt(discriminant));
  const double postiveNum = (-b + std::sqrt(discriminant));
  const double denominator = (2.0 * a);
  return {negativeNum / denominator, postiveNum / denominator};
}

template <typename T> inline T scaleToPositiveRange(T value) {
  return (value + 1.0) / 2.0; // Maps from [-1,1] to [0,1]
}
template <typename T> inline T scaleToSymmetricRange(T value) {
  return value * 2.0 - 1.0; // Maps from [0,1] to [-1,1]
}

inline double randomDouble() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double randomDouble(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * randomDouble();
}

} // namespace utils