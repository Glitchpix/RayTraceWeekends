#pragma once
#include "utils.hpp"
#include <array>
#include <ostream>

class Vec3 {
public:
  std::array<double, 3> e{};

  Vec3() = default;
  Vec3(double e1, double e2, double e3) : e{e1, e2, e3} {};

  [[nodiscard]] double x() const { return e[0]; }
  [[nodiscard]] double y() const { return e[1]; }
  [[nodiscard]] double z() const { return e[2]; }

  Vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
  double operator[](size_t i) const { return e.at(i); }
  double& operator[](size_t i) { return e.at(i); }

  Vec3& operator+=(const Vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  Vec3& operator*=(const Vec3& v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
  }

  Vec3& operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  Vec3& operator/=(double t) { return *this *= 1.0 / t; }

  [[nodiscard]] double length() const { return sqrt(length_squared()); }

  [[nodiscard]] double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  [[nodiscard]] bool near_zero() const {
    // Return true if the vector is close to zero in all dimensions.
    constexpr auto epsilon = 1e-8;
    return (std::fabs(e[0]) < epsilon) && (std::fabs(e[1]) < epsilon) &&
           (std::fabs(e[2]) < epsilon);
  }

  static Vec3 random() {
    return {utils::randomDouble(), utils::randomDouble(),
            utils::randomDouble()};
  }

  static Vec3 random(double min, double max) {
    return {utils::randomDouble(min, max), utils::randomDouble(min, max),
            utils::randomDouble(min, max)};
  }
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
  return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
  return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
  return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline Vec3 operator*(double t, const Vec3& v) {
  return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline Vec3 operator+(double t, const Vec3& v) {
  return {t + v.e[0], t + v.e[1], t + v.e[2]};
}

inline Vec3 operator+(const Vec3& v, double t) { return t + v; }

inline Vec3 operator*(const Vec3& v, double t) { return t * v; }

inline Vec3 operator/(const Vec3& v, double t) { return (1 / t) * v; }

inline double dot(const Vec3& u, const Vec3& v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
  return {u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2],
          u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline Vec3 unitVector(const Vec3& v) { return v / v.length(); }

inline Vec3 randomUnitVector() {
  while (true) {
    auto p = Vec3::random(-1, 1);
    auto lensq = p.length_squared();
    if (1e-160 < lensq && lensq <= 1) {
      return p / sqrt(lensq);
    }
  }
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
  Vec3 onUnitSphere = randomUnitVector();
  if (dot(onUnitSphere, normal) > 0) {
    return onUnitSphere;
  }
  return -onUnitSphere;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
  return v - 2 * dot(v, n) * n;
}