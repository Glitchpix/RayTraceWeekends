#pragma once

#include "camera.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "utils.hpp"
#include <memory>
namespace scene {
using std::make_shared;
using std::shared_ptr;
void defaultScene(HittableList& world, Camera& cam) {
  auto materialGround = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  auto materialCenter = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto materialLeft = make_shared<Dielectric>(1.51);
  auto materialBubble = make_shared<Dielectric>(1.00 / 1.51);
  auto materialRight = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

  world.add(
      make_shared<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0, materialGround));
  world.add(make_shared<Sphere>(Vec3(0.0, 0.0, -1.2), 0.5, materialCenter));
  world.add(make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.5, materialLeft));
  world.add(make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.4, materialBubble));
  world.add(make_shared<Sphere>(Vec3(1.0, 0.0, -1.0), 0.5, materialRight));

  constexpr double aspectRatio = 16.0 / 9.0;
  constexpr double cameraFov = 20;
  constexpr int imageWidth = 400;
  constexpr int samplesPerPixel = 10;
  constexpr int maxDepth = 20;

  cam.mAspectRatio = aspectRatio;
  cam.mImageWidth = imageWidth;
  cam.mSamplesPerPixel = samplesPerPixel;
  cam.mMaxDepth = maxDepth;

  cam.mVerticalFov = cameraFov;
  cam.mLookFrom = {-2, 2, 1};
  cam.mLookAt = {0, 0, -1};
  cam.mUp = {0, 1, 0};

  cam.mDefocusAngle = 10.0;
  cam.mFocusDistance = 3.4;
}

void twoOppositeSphere(HittableList& world) {
  auto R = std::cos(utils::PI / 4);

  auto material_left = make_shared<Lambertian>(Color(0, 0, 1));
  auto material_right = make_shared<Lambertian>(Color(1, 0, 0));

  world.add(make_shared<Sphere>(Vec3(-R, 0, -1), R, material_left));
  world.add(make_shared<Sphere>(Vec3(R, 0, -1), R, material_right));
}

void oneWeekendFinalScene(HittableList& world, Camera& cam) {
  auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = utils::randomDouble();
      Vec3 center(a + 0.9 * utils::randomDouble(), 0.2,
                  b + 0.9 * utils::randomDouble());

      if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<IMaterial> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Color::random() * Color::random();
          sphere_material = make_shared<Lambertian>(albedo);
          auto endCenter = center + Vec3{0, utils::randomDouble(0, 0.5), 0};
          world.add(
              make_shared<Sphere>(center, endCenter, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Color::random(0.5, 1);
          auto fuzz = utils::randomDouble(0, 0.5);
          sphere_material = make_shared<Metal>(albedo, fuzz);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<Dielectric>(1.5);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<Dielectric>(1.5);
  world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, material3));

  cam.mAspectRatio = 16.0 / 9.0;
  cam.mImageWidth = 400;
  cam.mSamplesPerPixel = 50;
  cam.mMaxDepth = 50;

  cam.mVerticalFov = 20;
  cam.mLookFrom = Vec3(13, 2, 3);
  cam.mLookAt = Vec3(0, 0, 0);
  cam.mUp = Vec3(0, 1, 0);

  cam.mDefocusAngle = 0.6;
  cam.mFocusDistance = 10.0;
}
}; // namespace scene