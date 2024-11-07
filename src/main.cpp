#include "camera.hpp"
#include "hittable_list.hpp"
#include "scene.hpp"

int main() {
  HittableList world{};

  scene::defaultScene(world);

  constexpr double aspectRatio = 16.0 / 9.0;
  constexpr double cameraFov = 20;
  constexpr int imageWidth = 400;
  constexpr int samplesPerPixel = 10;
  constexpr int maxDepth = 20;

  Camera cam;
  cam.mAspectRatio = aspectRatio;
  cam.mImageWidth = imageWidth;
  cam.mSamplesPerPixel = samplesPerPixel;
  cam.mMaxDepth = maxDepth;
  cam.mVerticalFov = cameraFov;
  cam.mLookFrom = {-2, 2, 1};
  cam.mLookAt = {0, 0, -1};
  cam.mUp = {0, 1, 0};

  cam.render(world);
}