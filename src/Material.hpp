#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hittable.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const {
        return false;
    }
};

class Lambertian : public Material {
private:
    Color albedo;

  public:
    Lambertian(const Color &albedo) : albedo(albedo) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override {
        // Lambertian distribution for diffuse scattering
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class Metal : public Material {
private:
    Color albedo;
    double fuzz;

public:
    Metal(const Color &albedo, double fuzz=0.0L) : albedo(albedo), fuzz(std::fmin(1.0L,std::fabs(fuzz))) {}

    bool scatter(const Ray &r_in, const HitRecord& rec, Color &attenuation, Ray &scattered)
    const override {
        Vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return true;
    }
};

#endif