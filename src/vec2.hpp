#pragma once

struct Vec2 {
  union {
    double x, u;
  };
  union {
    double y, v;
  };

  Vec2() = default;
  Vec2(double x, double y) : x{x}, y{y} {};
};