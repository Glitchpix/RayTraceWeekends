#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "vec3.hpp"
#include <cmath>
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
  bool scatter(const Ray& incoming, const HitRecord& hitInfo,
               Color& attenuation, Ray& scattered) const override {
    Vec3 scatterDirection = hitInfo.normal + randomUnitVector();
    if (scatterDirection.near_zero()) {
      scatterDirection = hitInfo.normal;
    }
    scattered = Ray{hitInfo.position, scatterDirection, incoming.time()};
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
    scattered = Ray{hitInfo.position, reflectDirection, incoming.time()};
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
    double refractIndex =
        hitInfo.frontFace ? (1.0 / mRefractionIndex) : mRefractionIndex;

    Vec3 incomingUnitDirection = unitVector(incoming.direction());

    double cosTheta =
        std::fmin(dot(-incomingUnitDirection, hitInfo.normal), 1.0);
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool totalInternalReflection = refractIndex * sinTheta > 1.0;
    Vec3 newDirection{};
    if (totalInternalReflection ||
        reflectance(cosTheta, refractIndex) > utils::randomDouble()) {
      newDirection = reflect(incomingUnitDirection, hitInfo.normal);
    } else {
      newDirection =
          refract(incomingUnitDirection, hitInfo.normal, refractIndex);
    }

    scattered = Ray{hitInfo.position, newDirection, incoming.time()};
    return true;
  }

private:
  double mRefractionIndex{};
  static double reflectance(double cosine, double refraction_index) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
  }
};