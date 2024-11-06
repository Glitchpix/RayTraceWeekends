#include "camera.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "vec3.hpp"
#include <memory>

int main() {
  HittableList world{};

  auto materialGround = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  auto materialCenter = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto materialLeft = std::make_shared<Metal>(Color(0.8, 0.8, 0.8));
  auto materialRight = std::make_shared<Metal>(Color(0.8, 0.6, 0.2));

  world.add(
      std::make_shared<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0, materialGround));
  world.add(
      std::make_shared<Sphere>(Vec3(0.0, 0.0, -1.2), 0.5, materialCenter));
  world.add(std::make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.5, materialLeft));
  world.add(std::make_shared<Sphere>(Vec3(1.0, 0.0, -1.0), 0.5, materialRight));

  constexpr double aspectRatio = 16.0 / 9.0;
  constexpr int imageWidth = 1000;
  constexpr int samplesPerPixel = 250;

  Camera cam;
  cam.mAspectRatio = aspectRatio;
  cam.mImageWidth = imageWidth;
  cam.mSamplesPerPixel = samplesPerPixel;

  cam.render(world);
}