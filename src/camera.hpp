#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include <cmath>
#include <iostream>

class Camera {
public:
  double mAspectRatio = 1.0;
  int mImageWidth = 100;
  int mSamplesPerPixel = 10;
  int mMaxDepth = 10;

  double mVerticalFov = 90;
  Vec3 mLookFrom{0, 0, 0};
  Vec3 mLookAt{0, 0, -1};
  Vec3 mUp{0, 1, 0};

  double mDefocusAngle = 0;
  double mFocusDistance = 10;

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
        pixelColor *= mPixelSampleScale;
        color::write(std::cout, pixelColor);
      }
    }

    std::clog << "\n\rDone.\n";
  }

private:
  int mImageHeight{};
  double mPixelSampleScale{};
  Vec3 mCenter;
  Vec3 mPixelDeltaX;
  Vec3 mPixelDeltaY;
  Vec3 mFirstPixelLocation;
  Vec3 mU, mV, mW;

  Vec3 mDefocusDiskU;
  Vec3 mDefocusDiskV;

  void initialize() {
    mImageHeight = std::max(int(mImageWidth / mAspectRatio), 1);

    mPixelSampleScale = 1.0 / mSamplesPerPixel;

    mCenter = mLookFrom;

    const double theta = utils::toRadians(mVerticalFov);
    const double h = std::tan(theta / 2.0);
    const double viewportHeight = 2.0 * h * mFocusDistance;
    const double viewportWidth =
        viewportHeight * (double(mImageWidth) / mImageHeight);

    mW = unitVector(mLookFrom - mLookAt);
    mU = unitVector(cross(mUp, mW));
    mV = cross(mW, mU);

    const Vec3 viewportXDirection = viewportWidth * mU;
    const Vec3 viewportYDirection = viewportHeight * -mV; // Flipped y-axis

    mPixelDeltaX = viewportXDirection / mImageWidth;
    mPixelDeltaY = viewportYDirection / mImageHeight;

    const auto viewportUpperLeft = mCenter - (mFocusDistance * mW) -
                                   viewportXDirection / 2 -
                                   viewportYDirection / 2;
    mFirstPixelLocation =
        viewportUpperLeft + 0.5 * (mPixelDeltaX + mPixelDeltaY);

    const auto defocusRadius =
        mFocusDistance * std::tan(utils::toRadians(mDefocusAngle / 2.0));

    mDefocusDiskU = mU * defocusRadius;
    mDefocusDiskV = mV * defocusRadius;
  };

  [[nodiscard]] Ray calculateSampleRay(int xIndex, int yIndex) const {

    const Vec3 offset = samplePixelCenterOffset();
    const Vec3 centerDelta = ((yIndex + offset.y()) * mPixelDeltaY) +
                             ((xIndex + offset.x()) * mPixelDeltaX);
    const Vec3 pixelCenter = mFirstPixelLocation + centerDelta;

    const Vec3 rayOrigin = (mDefocusAngle <= 0) ? mCenter : defocusDiskSample();
    const Vec3 rayDirection = pixelCenter - rayOrigin;
    const double rayTime = utils::randomDouble();

    return {rayOrigin, rayDirection, rayTime};
  }

  [[nodiscard]] Vec3 defocusDiskSample() const {
    const Vec3 point = randomInUnitDisk();
    return mCenter + (point.x() * mDefocusDiskU) + (point.y() * mDefocusDiskV);
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
    return (1.0 - interpolant) * color::White +
           interpolant * Color(0.5, 0.7, 1.0);
  }
};