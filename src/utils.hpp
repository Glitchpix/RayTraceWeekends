#pragma once

#include <cmath>
namespace utils {

inline double quadraticRealSolve(double a, double b, double c) {
  const double discriminant = b * b - 4 * a * c;
  if (discriminant < 0.0) {
    return -1.0;
  }
  return (-b - std::sqrt(discriminant)) / (2.0 * a);
}

template <typename T> inline T scaleToPositiveRange(T value) {
  return (value + 1.0) / 2.0; // Maps from [-1,1] to [0,1]
}
template <typename T> inline T scaleToSymmetricRange(T value) {
  return value * 2.0 - 1.0; // Maps from [0,1] to [-1,1]
}

} // namespace utils