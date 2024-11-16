#pragma once

#include "aabb.hpp"
#include "interval.hpp"
#include "ray.hpp"
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