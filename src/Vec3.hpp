#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.hpp"
#include <cmath>
#include <iostream>

class Vec3 {
private:
    double e[3];

public:
    Vec3() : e{0,0,0} {}
    Vec3(double x, double y, double z) : e{x, y, z} {}

    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    Vec3 operator-() const {return Vec3(-e[0], -e[1], -e[2]);}
    double operator[](unsigned i) const {return e[i];}
    double& operator[](unsigned i) {return e[i];}

    Vec3& operator+=(const Vec3& v) {
         e[0] += v.e[0];
         e[1] += v.e[1];
         e[2] += v.e[2];
         return *this;
    }

    Vec3& operator*=(double t) {
         e[0] *= t;
         e[1] *= t;
         e[2] *= t;
         return *this;
    }

    Vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        constexpr auto S = 1e-8;
        return (std::fabs(e[0]) < S) && (std::fabs(e[1]) < S) && (std::fabs(e[2]) < S);
    }

    static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    static Vec3 random(double min, double max) {
        return Vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;
using Color = Vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3 &v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x()+v.x(), u.y()+v.y(), u.z()+v.z());
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x()-v.x(), u.y()-v.y(), u.z()-v.z());
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x()*v.x(), u.y()*v.y(), u.z()*v.z());
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t*v.x(), t*v.y(), t*v.z());
}

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
}

inline Vec3 operator/(const Vec3 &v, double t) {
    return (1/t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.x()*v.x()
         + u.y()*v.y()
         + u.z()*v.z();
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.y()*v.z() - u.z()*v.y(),
                u.z()*v.x() - u.x()*v.z(),
                u.x()*v.y() - u.y()*v.x());
}

inline Vec3 unit_vector(const Vec3 &v) {
    return v / v.length();
}

inline Vec3 random_unit_vector() {
    while (true) {
        auto p = Vec3::random(-1,1);
        auto len_sq = p.length_squared();
        if (1e-160 < len_sq && len_sq <= 1)
            return p / sqrt(len_sq);
    }
}

inline Vec3 random_on_hemisphere(const Vec3 &normal) {
    Vec3 on_unit_sphere = random_unit_vector();
    return (dot(on_unit_sphere, normal) > 0.0) ? // In the same hemisphere as the normal
        on_unit_sphere : -on_unit_sphere;
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n) {
    return v - 2*dot(v,n)*n;
}

#endif