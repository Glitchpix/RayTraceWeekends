#pragma once

#include "utils.hpp"

class Interval {
public:
  constexpr Interval()
      : mMin{+utils::INFINITE_DOUBLE}, mMax{-utils::INFINITE_DOUBLE} {};

  constexpr Interval(double min, double max) : mMin{min}, mMax{max} {};

  constexpr Interval(const Interval& first, const Interval& second)
      : mMin(first.min() <= second.min() ? first.min() : second.min()),
        mMax(first.max() >= second.max() ? first.max() : second.max()) {}

  [[nodiscard]] double min() const { return mMin; }
  [[nodiscard]] double max() const { return mMax; }

  [[nodiscard]] double size() const { return mMax - mMin; }

  [[nodiscard]] bool contains(double x) const { return mMin <= x && x <= mMax; }

  [[nodiscard]] bool surrounds(double x) const { return mMin < x && x < mMax; }

  [[nodiscard]] constexpr double clamp(double x) const {
    return std::clamp(x, mMin, mMax);
  }

  [[nodiscard]] Interval expand(double delta) const {
    auto padding = delta / 2;
    return {mMin - padding, mMax + padding};
  }

  static const Interval empty, universe;

private:
  double mMin, mMax;
};

constexpr auto Interval::empty = Interval{};
constexpr auto Interval::universe =
    Interval{-utils::INFINITE_DOUBLE, +utils::INFINITE_DOUBLE};

Interval operator+(const Interval& interval, double displacement) {
  return {interval.min() + displacement, interval.max() + displacement};
}

Interval operator+(double displacement, const Interval& interval) {
  return interval + displacement;
}