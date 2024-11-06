#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "vec3.hpp"
#include <memory>

int main() {
  HittableList world{};
  world.add(std::make_shared<Sphere>(Vec3{0, 0, -1}, 0.5));
  world.add(std::make_shared<Sphere>(Vec3{0, -100.5, -1}, 100));

  constexpr double aspectRatio = 16.0 / 9.0;
  constexpr int imageWidth = 500;
  constexpr int samplesPerPixel = 100;

  Camera cam;
  cam.mAspectRatio = aspectRatio;
  cam.mImageWidth = imageWidth;
  cam.mSamplesPerPixel = samplesPerPixel;

  cam.render(world);
}