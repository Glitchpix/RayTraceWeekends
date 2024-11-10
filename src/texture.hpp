#pragma once

#include "src/color.hpp"
class Texture {
public:
  [[nodiscard]] virtual Color value(double u, double v,
                                    const Vec3& point) const = 0;
  virtual ~Texture() = default;

protected:
  Texture() = default;
  Texture(const Texture&) = default;
  Texture(Texture&&) = default;
  Texture& operator=(const Texture&) = default;
  Texture& operator=(Texture&&) = default;
};

class SolidColor : public Texture {
public:
  SolidColor(const Color& albedo) : mAlbedo{albedo} {}

  SolidColor(double red, double green, double blue)
      : SolidColor(Color{red, green, blue}) {}

  [[nodiscard]] Color value(double u, double v,
                            const Vec3& point) const override {
    (void)u;
    (void)v;
    (void)point;
    return mAlbedo;
  }

private:
  Color mAlbedo;
};

class CheckerTexture : public Texture {
public:
  CheckerTexture(double scale, std::shared_ptr<Texture> even,
                 std::shared_ptr<Texture> odd)
      : mInverseScale{1.0 / scale}, mEven{even}, mOdd{odd} {}

  CheckerTexture(double scale, const Color& c1, const Color& c2)
      : CheckerTexture(scale, std::make_shared<SolidColor>(c1),
                       std::make_shared<SolidColor>(c2)) {}

  [[nodiscard]] Color value(double u, double v,
                            const Vec3& point) const override {
    auto xInteger = int(std::floor(mInverseScale * point.x()));
    auto yInteger = int(std::floor(mInverseScale * point.y()));
    auto zInteger = int(std::floor(mInverseScale * point.z()));

    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

    return isEven ? mEven->value(u, v, point) : mOdd->value(u, v, point);
  }

private:
  double mInverseScale;
  std::shared_ptr<Texture> mEven;
  std::shared_ptr<Texture> mOdd;
};