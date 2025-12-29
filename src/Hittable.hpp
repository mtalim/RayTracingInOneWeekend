#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.hpp"
#include "Interval.hpp"

class HitRecord {
private:
    // Point3 p;

public:
    double t;
    bool front_face;
    Vec3 normal;

    void set_face_normal(const Ray &r, const Vec3 &outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: The outward_normal is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
  public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const = 0;
};

#endif