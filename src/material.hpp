#pragma once

#include "color.hpp"
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
  Metal(const Color& albedo, double fuzz)
      : mAlbedo{albedo}, mFuzz{fuzz < 1.0 ? fuzz : 1.0} {};
  bool scatter(const Ray& incoming, const HitRecord& hitInfo,
               Color& attenuation, Ray& scattered) const override {
    Vec3 reflectDirection =
        unitVector(reflect(incoming.direction(), hitInfo.normal)) +
        (mFuzz * randomUnitVector());
    scattered = Ray{hitInfo.position, reflectDirection};
    attenuation = mAlbedo;
    return (dot(scattered.direction(), hitInfo.normal) > 0);
  }

private:
  Color mAlbedo;
  double mFuzz{};
};

class Dielectric : public IMaterial {
public:
  Dielectric(double refractionIndex) : mRefractionIndex{refractionIndex} {};
  bool scatter(const Ray& incoming, const HitRecord& hitInfo,
               Color& attenuation, Ray& scattered) const override {
    attenuation = color::White;
    double ri = hitInfo.frontFace ? (1.0 / mRefractionIndex) : mRefractionIndex;

    Vec3 incomingDirection = unitVector(incoming.direction());
    Vec3 refracted = refract(incomingDirection, hitInfo.normal, ri);

    scattered = Ray{hitInfo.position, refracted};
    return true;
  }

private:
  double mRefractionIndex{};
};