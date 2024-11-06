#include "color.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vec3.hpp"
#include <iostream>
#include <memory>

Color getColor(const Ray &ray, const Hittable &world) {
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

int main() {
  constexpr double aspectRatio = 16.0 / 9.0;
  constexpr int imageWidth = 800;
  constexpr int imageHeight = std::max(int(imageWidth / aspectRatio), 1);

  constexpr double focalLength = 1.0;
  constexpr double viewportHeight = 2.0;
  constexpr double viewportWidth =
      viewportHeight * (double(imageWidth) / imageHeight);
  const Vec3 cameraCenter = Vec3{0, 0, 0};

  const Vec3 viewportXDirection = Vec3{viewportWidth, 0, 0};
  const Vec3 viewportYDirection = Vec3{0, -viewportHeight, 0};

  const auto pixelDeltaX = viewportXDirection / imageWidth;
  const auto pixelDeltaY = viewportYDirection / imageHeight;

  const auto viewportUpperLeft = cameraCenter - Vec3{0, 0, focalLength} -
                                 viewportXDirection / 2 -
                                 viewportYDirection / 2;
  const auto firstPixelLocation =
      viewportUpperLeft + 0.5 * (pixelDeltaX + pixelDeltaY);

  HittableList world{};
  world.add(std::make_shared<Sphere>(Vec3{0, 0, -1}, 0.5));
  world.add(std::make_shared<Sphere>(Vec3{0, -100.5, -1}, 100));

  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (int yIndex = 0; yIndex < imageHeight; ++yIndex) {
    std::clog << "\rScanlines remaining: " << (imageHeight - yIndex) << ' '
              << std::flush;
    for (int xIndex = 0; xIndex < imageWidth; ++xIndex) {
      auto pixelCenter =
          firstPixelLocation + (yIndex * pixelDeltaY) + (xIndex * pixelDeltaX);
      auto rayDirection = pixelCenter - cameraCenter;
      Ray r{cameraCenter, rayDirection};

      auto pixelColor = getColor(r, world);
      color::write(std::cout, pixelColor);
    }
  }

  std::clog << "\n\rDone.\n";
}