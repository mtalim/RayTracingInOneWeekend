#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>
#include <fstream>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

constexpr double INF = std::numeric_limits<double>::infinity();
constexpr double PI = 3.1415926535897932385L;

// Utility Functions

constexpr double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0L;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

// Common Headers

#include "Ray.hpp"
#include "Vec3.hpp"
#include "Interval.hpp"

#endif