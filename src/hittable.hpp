#pragma once

#include "aabb.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "utils.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include <memory>

class IMaterial;

struct HitRecord {
  Vec3 position;
  std::shared_ptr<IMaterial> material;
  double t{};
  Vec2<double> uv;

  void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
    frontFace = dot(ray.direction(), outwardNormal) < 0;
    mNormal = frontFace ? outwardNormal : -outwardNormal;
  }

  [[nodiscard]] const Vec3& normal() const { return mNormal; }
  [[nodiscard]] bool frontFacing() const { return frontFace; }

private:
  bool frontFace{};
  Vec3 mNormal;
};

class Hittable {
public:
  Hittable() = default;
  virtual ~Hittable() = default;

  virtual bool hit(const Ray& ray, Interval rayRange,
                   HitRecord& hitInfo) const = 0;
  [[nodiscard]] virtual AABB boundingBox() const = 0;

protected:
  Hittable(const Hittable&) = default;
  Hittable(Hittable&&) = default;
  Hittable& operator=(const Hittable&) = default;
  Hittable& operator=(Hittable&&) = default;
};

class Translate : public Hittable {
public:
  Translate(std::shared_ptr<Hittable> object, const Vec3& offset)
      : mObject{object}, mOffset{offset},
        mBoundingBox(object->boundingBox() + offset) {}
  bool hit(const Ray& ray, Interval rayRange,
           HitRecord& hitInfo) const override {
    Ray offsetRay{ray.origin() - mOffset, ray.direction(), ray.time()};

    if (!mObject->hit(offsetRay, rayRange, hitInfo)) {
      return false;
    }

    hitInfo.position += mOffset;
    return true;
  }

  [[nodiscard]] AABB boundingBox() const override { return mBoundingBox; }

private:
  std::shared_ptr<Hittable> mObject;
  Vec3 mOffset;
  AABB mBoundingBox;
};

class RotateY : public Hittable {
  // TODO: Refactor to use proper rotation matrix
public:
  RotateY(std::shared_ptr<Hittable> object, double angle) : mObject{object} {
    auto radians = utils::toRadians(angle);
    sinTheta = std::sin(radians);
    cosTheta = std::cos(radians);
    mBoundingBox = mObject->boundingBox();

    Vec3 min{utils::INFINITE_DOUBLE};
    Vec3 max{-utils::INFINITE_DOUBLE};

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          auto x = i * mBoundingBox.mX.max() + (1 - i) * mBoundingBox.mX.min();
          auto y = j * mBoundingBox.mY.max() + (1 - j) * mBoundingBox.mY.min();
          auto z = k * mBoundingBox.mZ.max() + (1 - k) * mBoundingBox.mZ.min();

          auto newx = cosTheta * x + sinTheta * z;
          auto newz = -sinTheta * x + cosTheta * z;

          Vec3 tester(newx, y, newz);

          for (size_t c = 0; c < 3; c++) {
            min[c] = std::fmin(min[c], tester[c]);
            max[c] = std::fmax(max[c], tester[c]);
          }
        }
      }
    }

    mBoundingBox = AABB{min, max};
  }
  bool hit(const Ray& ray, Interval rayRange,
           HitRecord& hitInfo) const override {
    // Transform the ray from world space to object space.

    auto origin =
        Vec3((cosTheta * ray.origin().x()) - (sinTheta * ray.origin().z()),
             ray.origin().y(),
             (sinTheta * ray.origin().x()) + (cosTheta * ray.origin().z()));

    auto direction = Vec3(
        (cosTheta * ray.direction().x()) - (sinTheta * ray.direction().z()),
        ray.direction().y(),
        (sinTheta * ray.direction().x()) + (cosTheta * ray.direction().z()));

    Ray rotatedRay(origin, direction, ray.time());

    // Determine whether an intersection exists in object space (and if so,
    // where).

    if (!mObject->hit(rotatedRay, rayRange, hitInfo)) {
      return false;
    }

    // Transform the intersection from object space back to world space.

    hitInfo.position = Vec3{
        (cosTheta * hitInfo.position.x()) + (sinTheta * hitInfo.position.z()),
        hitInfo.position.y(),
        (-sinTheta * hitInfo.position.x()) + (cosTheta * hitInfo.position.z())};

    const auto worldNormal = Vec3{
        (cosTheta * hitInfo.normal().x()) + (sinTheta * hitInfo.normal().z()),
        hitInfo.normal().y(),
        (-sinTheta * hitInfo.normal().x()) + (cosTheta * hitInfo.normal().z())};
    hitInfo.setFaceNormal(ray, worldNormal);

    return true;
  }

  [[nodiscard]] AABB boundingBox() const override { return mBoundingBox; }

private:
  std::shared_ptr<Hittable> mObject;
  double sinTheta;
  double cosTheta;
  AABB mBoundingBox;
};