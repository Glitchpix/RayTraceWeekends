#pragma once

#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "utils.hpp"
#include <memory>
namespace scene {
using std::make_shared;
void defaultScene(HittableList& world) {
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
}

void twoOppositeSphere(HittableList& world) {
  auto R = std::cos(utils::PI / 4);

  auto material_left = make_shared<Lambertian>(Color(0, 0, 1));
  auto material_right = make_shared<Lambertian>(Color(1, 0, 0));

  world.add(make_shared<Sphere>(Vec3(-R, 0, -1), R, material_left));
  world.add(make_shared<Sphere>(Vec3(R, 0, -1), R, material_right));
}
}; // namespace scene