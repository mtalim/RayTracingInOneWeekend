#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.hpp"
#include "Vec3.hpp"
#include "Interval.hpp"

class Sphere : public Hittable {
private:
    Point3 center;
    double radius;

public:
    Sphere(const Point3 &center, double radius) : center(center), radius(std::fmax(0.0L,radius)) {}

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override {
        Vec3 oc = center - r.origin();
        auto a = r.direction().length_squared(); // equivalent of dot-product with self
        auto h = dot(r.direction(), oc); // b was simplified in calculation.
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        // Check if t has any real value along the ray, i.e. non-negative discriminant sqrt()
        if (discriminant < 0)
            return false;

        auto sqrt_det = std::sqrt(discriminant);

        auto root = (h - sqrt_det) / a;
        if (root <= ray_t.min) {
            root = (h + sqrt_det) / a;
            if (root <= ray_t.min)
                return false;
        }
        if (root >= ray_t.max)
            return false;

        rec.t = root;
        // rec.p = r.at(rec.t);
        Vec3 outward_normal = (r.at(rec.t) - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }
};

#endif