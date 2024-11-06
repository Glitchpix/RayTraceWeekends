#pragma once

#include "utils.hpp"

class Interval {
public:
  constexpr Interval()
      : mMin{+utils::INFINITE_DOUBLE}, mMax{-utils::INFINITE_DOUBLE} {};

  constexpr Interval(double min, double max) : mMin{min}, mMax{max} {};

  [[nodiscard]] double min() const { return mMin; }
  [[nodiscard]] double max() const { return mMax; }

  [[nodiscard]] double size() const { return mMax - mMin; }

  [[nodiscard]] bool contains(double x) const { return mMin <= x && x <= mMax; }

  [[nodiscard]] bool surrounds(double x) const { return mMin < x && x < mMax; }

  [[nodiscard]] constexpr double clamp(double x) const {
    return std::clamp(x, mMin, mMax);
  }

  static const Interval empty, universe;

private:
  double mMin, mMax;
};

const auto Interval::empty = Interval{};
const auto Interval::universe =
    Interval{-utils::INFINITE_DOUBLE, +utils::INFINITE_DOUBLE};