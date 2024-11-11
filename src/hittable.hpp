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
  Vec3 normal;
  std::shared_ptr<IMaterial> material;
  double t{};
  Vec2 uv;
  bool frontFace{};

  void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
    frontFace = dot(ray.direction(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
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