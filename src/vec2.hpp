#pragma once

template <class T = double>
struct Vec2 {
  union {
    T x, u;
  };
  union {
    T y, v;
  };

  Vec2() = default;
  Vec2(T x, T y) : x{x}, y{y} {};
};