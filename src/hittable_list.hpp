#pragma once

#include "hittable.hpp"
#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
  HittableList() = default;
  HittableList(std::shared_ptr<Hittable> object) { add(object); }

  void clear() { mObjects.clear(); }

  void add(std::shared_ptr<Hittable> object) { mObjects.push_back(object); }

  bool hit(const Ray &ray, double tMin, double tMax,
           HitRecord &rec) const override {
    HitRecord tempInfo;
    bool hitAnything = false;
    auto closestSoFar = tMax;

    for (const auto &object : mObjects) {
      if (object->hit(ray, tMin, closestSoFar, tempInfo)) {
        hitAnything = true;
        closestSoFar = tempInfo.t;
        rec = tempInfo;
      }
    }

    return hitAnything;
  }

private:
  std::vector<std::shared_ptr<Hittable>> mObjects;
};