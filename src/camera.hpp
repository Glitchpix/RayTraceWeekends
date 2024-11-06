#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include <iostream>
class Camera {
public:
  double mAspectRatio = 1.0;
  double mImageWidth = 100;

  void render(const Hittable &world) {
    initialize();
    std::cout << "P3\n" << mImageWidth << ' ' << mImageHeight << "\n255\n";

    for (int yIndex = 0; yIndex < mImageHeight; ++yIndex) {
      std::clog << "\rScanlines remaining: " << (mImageHeight - yIndex) << ' '
                << std::flush;
      for (int xIndex = 0; xIndex < mImageWidth; ++xIndex) {
        auto pixelCenter = mFirstPixelLocation + (yIndex * mPixelDeltaY) +
                           (xIndex * mPixelDeltaX);
        auto rayDirection = pixelCenter - mCenter;
        Ray r{mCenter, rayDirection};

        auto pixelColor = calculateRayColor(r, world);
        color::write(std::cout, pixelColor);
      }
    }

    std::clog << "\n\rDone.\n";
  }

private:
  int mImageHeight;
  Vec3 mCenter;
  Vec3 mPixelDeltaX;
  Vec3 mPixelDeltaY;
  Vec3 mFirstPixelLocation;

  void initialize() {
    mImageHeight = std::max(int(mImageWidth / mAspectRatio), 1);

    constexpr double focalLength = 1.0;
    constexpr double viewportHeight = 2.0;
    const double viewportWidth =
        viewportHeight * (double(mImageWidth) / mImageHeight);
    mCenter = Vec3{0, 0, 0};

    const Vec3 viewportXDirection = Vec3{viewportWidth, 0, 0};
    const Vec3 viewportYDirection = Vec3{0, -viewportHeight, 0};

    mPixelDeltaX = viewportXDirection / mImageWidth;
    mPixelDeltaY = viewportYDirection / mImageHeight;

    const auto viewportUpperLeft = mCenter - Vec3{0, 0, focalLength} -
                                   viewportXDirection / 2 -
                                   viewportYDirection / 2;
    mFirstPixelLocation =
        viewportUpperLeft + 0.5 * (mPixelDeltaX + mPixelDeltaY);
  };

  Color calculateRayColor(const Ray &ray, const Hittable &world) {
    HitRecord hitInfo;
    if (world.hit(ray, Interval{0, utils::INFINITE_DOUBLE}, hitInfo)) {
      Vec3 normal = hitInfo.normal;
      Color normalColor{normal.x(), normal.y(), normal.z()};
      return utils::scaleToPositiveRange(normalColor);
    }
    Vec3 unitDirection = unit_vector(ray.direction());
    Color white{1, 1, 1};
    Color blue{0, 0, 1};

    auto interpolant = utils::scaleToPositiveRange(unitDirection.y());
    return (1.0 - interpolant) * white + interpolant * blue;
  }
};