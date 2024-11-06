#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include <iostream>

class Camera {
public:
  double mAspectRatio = 1.0;
  double mImageWidth = 100;
  double mSamplesPerPixel = 10;
  int mMaxDepth = 10;

  void render(const Hittable& world) {
    initialize();
    std::cout << "P3\n" << mImageWidth << ' ' << mImageHeight << "\n255\n";

    for (int yIndex = 0; yIndex < mImageHeight; ++yIndex) {
      std::clog << "\rScanlines remaining: " << (mImageHeight - yIndex) << ' '
                << std::flush;
      for (int xIndex = 0; xIndex < mImageWidth; ++xIndex) {
        auto pixelColor = Color{0, 0, 0};
        for (int iSample = 0; iSample < mSamplesPerPixel; ++iSample) {
          Ray r = calculateSampleRay(xIndex, yIndex);
          pixelColor += calculateRayColor(r, mMaxDepth, world);
        }
        pixelColor *= pixelSampleScale;
        color::write(std::cout, pixelColor);
      }
    }

    std::clog << "\n\rDone.\n";
  }

private:
  int mImageHeight{};
  double pixelSampleScale{};
  Vec3 mCenter;
  Vec3 mPixelDeltaX;
  Vec3 mPixelDeltaY;
  Vec3 mFirstPixelLocation;

  void initialize() {
    mImageHeight = std::max(int(mImageWidth / mAspectRatio), 1);

    pixelSampleScale = 1.0 / mSamplesPerPixel;
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

  [[nodiscard]] Ray calculateSampleRay(int xIndex, int yIndex) const {

    const Vec3 offset = samplePixelCenterOffset();
    const Vec3 centerDelta = ((yIndex + offset.y()) * mPixelDeltaY) +
                             ((xIndex + offset.x()) * mPixelDeltaX);
    const Vec3 pixelCenter = mFirstPixelLocation + centerDelta;

    const Vec3 rayOrigin = mCenter;
    const Vec3 rayDirection = pixelCenter - rayOrigin;
    Ray r{rayOrigin, rayDirection};

    return r;
  }

  [[nodiscard]] static Vec3 samplePixelCenterOffset() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit
    // square.
    return {utils::randomDouble() - 0.5, utils::randomDouble() - 0.5, 0};
  }

  static Color calculateRayColor(const Ray& ray, int depth,
                                 const Hittable& world) {
    if (depth <= 0) {
      return color::Black;
    }
    HitRecord hitInfo;
    if (world.hit(ray, Interval{0.001, utils::INFINITE_DOUBLE}, hitInfo)) {
      Ray scattered{};
      Color attenuation{};
      if (hitInfo.material->scatter(ray, hitInfo, attenuation, scattered)) {
        return attenuation * calculateRayColor(scattered, depth - 1, world);
      }
      return color::Black;
    }
    Vec3 unitDirection = unitVector(ray.direction());

    auto interpolant = utils::scaleToPositiveRange(unitDirection.y());
    return (1.0 - interpolant) * color::White + interpolant * color::Blue;
  }
};