#pragma once

template <class T = int>
struct TVec3 {
  union {
    T x, u;
  };
  union {
    T y, v;
  };
  union {
    T z, w;
  };

  TVec3() = default;
  TVec3(T x, T y, T z) : x{x}, y{y}, z{z} {};
};