#pragma once

#include "color.hpp"
#include "image.hpp"
#include "interval.hpp"
#include "vec2.hpp"
class Texture {
public:
  [[nodiscard]] virtual Color value(const Vec2& uvCoords,
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

  [[nodiscard]] Color value(const Vec2& uvCoords,
                            const Vec3& point) const override {
    (void)uvCoords;
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

  [[nodiscard]] Color value(const Vec2& uvCoords,
                            const Vec3& point) const override {
    auto xInteger = int(std::floor(mInverseScale * point.x()));
    auto yInteger = int(std::floor(mInverseScale * point.y()));
    auto zInteger = int(std::floor(mInverseScale * point.z()));

    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

    return isEven ? mEven->value(uvCoords, point)
                  : mOdd->value(uvCoords, point);
  }

private:
  double mInverseScale;
  std::shared_ptr<Texture> mEven;
  std::shared_ptr<Texture> mOdd;
};

class UVTexture : public Texture {
public:
  [[nodiscard]] Color value(const Vec2& uvCoords,
                            const Vec3& point) const override {
    (void)point;
    return Color{uvCoords.u, uvCoords.v, 0.0};
  }
};

class ImageTexture : public Texture {
public:
  ImageTexture(const char* filename) : mImage(filename){};

  [[nodiscard]] Color value(const Vec2& uvCoords,
                            const Vec3& point) const override {
    (void)point;
    if (mImage.height() <= 0) {
      return mDebugColor;
    }

    double u = uvCoords.u;
    double v = uvCoords.v;

    u = Interval{0, 1}.clamp(u);
    v = 1.0 - Interval{0, 1}.clamp(v);

    auto uIntCoord = int(u * mImage.width());
    auto vIntCoord = int(v * mImage.height());

    auto pixelData = mImage.pixelData(uIntCoord, vIntCoord);

    auto r = pixelData[0];
    auto g = pixelData[1];
    auto b = pixelData[2];
    return {r, g, b};
  }

private:
  static constexpr Color mDebugColor = Color{0, 1, 1};
  Image mImage;
};