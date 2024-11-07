#pragma once

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <memory>

class IMaterial;

struct HitRecord {
  Vec3 position;
  Vec3 normal;
  std::shared_ptr<IMaterial> material;
  double t{};
  bool frontFace{};

  void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
    frontFace = dot(ray.direction(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

class Hittable {
public:
  Hittable() = default;
  Hittable(const Hittable&) = delete;
  Hittable(Hittable&&) = delete;
  Hittable& operator=(const Hittable&) = delete;
  Hittable& operator=(Hittable&&) = delete;
  virtual ~Hittable() = default;

  virtual bool hit(const Ray& ray, Interval rayRange,
                   HitRecord& hitInfo) const = 0;
};