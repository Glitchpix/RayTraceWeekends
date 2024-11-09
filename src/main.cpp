#include "camera.hpp"
#include "hittable_list.hpp"
#include "scene.hpp"
#include <chrono>
#include <iostream>

int main() {
  HittableList world{};
  Camera cam;

  scene::oneWeekendFinalScene(world, cam);

  auto t1 = std::chrono::high_resolution_clock::now();
  cam.render(world);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto ms_int = duration_cast<std::chrono::milliseconds>(t2 - t1);
  std::clog << "Rendering took: " << ms_int << " milliseconds.\n";
}