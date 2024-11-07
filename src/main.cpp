#include "camera.hpp"
#include "hittable_list.hpp"
#include "scene.hpp"

int main() {
  HittableList world{};
  Camera cam;
  scene::oneWeekendFinalScene(world, cam);

  cam.render(world);
}