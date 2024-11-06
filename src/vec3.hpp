#pragma once
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

inline Vec3 unit_vector(const Vec3& v) { return v / v.length(); }