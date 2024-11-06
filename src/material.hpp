#pragma once

#include "hittable.hpp"
#include "vec3.hpp"
class IMaterial {
public:
  IMaterial() = default;
  IMaterial(const IMaterial&) = delete;
  IMaterial(IMaterial&&) = delete;
  IMaterial& operator=(const IMaterial&) = delete;
  IMaterial& operator=(IMaterial&&) = delete;
  virtual ~IMaterial() = default;

  virtual bool scatter(const Ray& incoming, const HitRecord& hitInfo,
                       Color& attenuation, Ray& scattered) const {
    (void)incoming;
    (void)hitInfo;
    (void)attenuation;
    (void)scattered;
    return false;
  }
};

class Lambertian : public IMaterial {
public:
  Lambertian(const Color& albedo) : mAlbedo{albedo} {};
  bool scatter(const Ray& /**/, const HitRecord& hitInfo, Color& attenuation,
               Ray& scattered) const override {
    Vec3 scatterDirection = hitInfo.normal + randomUnitVector();
    if (scatterDirection.near_zero()) {
      scatterDirection = hitInfo.normal;
    }
    scattered = Ray{hitInfo.position, scatterDirection};
    attenuation = mAlbedo;
    return true;
  }

private:
  Color mAlbedo;
};

class Metal : public IMaterial {
public:
  Metal(const Color& albedo) : mAlbedo{albedo} {};
  bool scatter(const Ray& incoming, const HitRecord& hitInfo,
               Color& attenuation, Ray& scattered) const override {
    Vec3 reflectDirection = reflect(incoming.direction(), hitInfo.normal);
    scattered = Ray{hitInfo.position, reflectDirection};
    attenuation = mAlbedo;
    return true;
  }

private:
  Color mAlbedo;
};