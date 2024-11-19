#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
  HittableList() = default;
  HittableList(std::shared_ptr<Hittable> object) { add(object); }

  void clear() { mObjects.clear(); }

  void add(std::shared_ptr<Hittable> object) {
    mObjects.push_back(object);
    mBoundingBox = AABB{mBoundingBox, object->boundingBox()};
  }

  bool hit(const Ray& ray, Interval rayRange, HitRecord& rec) const override {
    HitRecord tempInfo;
    bool hitAnything = false;
    auto closestSoFar = rayRange.max();

    for (const auto& object : mObjects) {
      if (object->hit(ray, Interval{rayRange.min(), closestSoFar}, tempInfo)) {
        hitAnything = true;
        closestSoFar = tempInfo.t;
        rec = tempInfo;
      }
    }

    return hitAnything;
  }

  [[nodiscard]] AABB boundingBox() const override { return mBoundingBox; }

  [[nodiscard]] auto& getObjects() { return mObjects; }

private:
  std::vector<std::shared_ptr<Hittable>> mObjects;
  AABB mBoundingBox;
};