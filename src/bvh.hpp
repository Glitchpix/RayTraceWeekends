#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "interval.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>
class BVHNode : public Hittable {
public:
  BVHNode(HittableList list)
      : BVHNode{list.getObjects(), 0, list.getObjects().size()} {}

  BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start,
          size_t end)
      : mBoundingBox(AABB::empty) {

    for (size_t iObject = start; iObject < end; ++iObject) {
      mBoundingBox = AABB{mBoundingBox, objects[iObject]->boundingBox()};
    }

    size_t axis = mBoundingBox.longestAxis();

    auto comparator = (axis == 0)   ? boundingXCompare
                      : (axis == 1) ? boundingYCompare
                                    : boundingZCompare;

    auto objectSpan = end - start;

    if (objectSpan == 1) {
      mLeft = mRight = objects[start];
    } else if (objectSpan == 2) {
      mLeft = objects[start];
      mRight = objects[start + 1];
    } else {
      std::sort(std::begin(objects) + static_cast<long long>(start),
                std::begin(objects) + static_cast<long long>(end), comparator);

      auto mid = start + objectSpan / 2;
      mLeft = std::make_shared<BVHNode>(objects, start, mid);
      mRight = std::make_shared<BVHNode>(objects, mid, end);
    }
  }

  bool hit(const Ray& incoming, Interval rayRange,
           HitRecord& hitInfo) const override {
    if (!mBoundingBox.hit(incoming, rayRange)) {
      return false;
    }
    bool hitLeft = mLeft->hit(incoming, rayRange, hitInfo);
    rayRange = Interval{rayRange.min(), hitLeft ? hitInfo.t : rayRange.max()};
    bool hitRight = mRight->hit(incoming, rayRange, hitInfo);

    return hitLeft || hitRight;
  }

  [[nodiscard]] AABB boundingBox() const override { return mBoundingBox; }

private:
  std::shared_ptr<Hittable> mLeft;
  std::shared_ptr<Hittable> mRight;
  AABB mBoundingBox;

  static bool box_compare(const std::shared_ptr<Hittable> a,
                          const std::shared_ptr<Hittable> b,
                          size_t axis_index) {
    auto a_axis_interval = a->boundingBox().axisInterval(axis_index);
    auto b_axis_interval = b->boundingBox().axisInterval(axis_index);
    return a_axis_interval.min() < b_axis_interval.min();
  }

  static bool boundingXCompare(const std::shared_ptr<Hittable> a,
                               const std::shared_ptr<Hittable> b) {
    return box_compare(a, b, 0);
  }

  static bool boundingYCompare(const std::shared_ptr<Hittable> a,
                               const std::shared_ptr<Hittable> b) {
    return box_compare(a, b, 1);
  }

  static bool boundingZCompare(const std::shared_ptr<Hittable> a,
                               const std::shared_ptr<Hittable> b) {
    return box_compare(a, b, 2);
  }
};