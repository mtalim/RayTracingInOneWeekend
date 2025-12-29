#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.hpp"

class Interval {
  public:
    double min, max;

    Interval() : min(+INF), max(-INF) {} // Default interval is empty

    Interval(double min, double max) : min(min), max(max) {}

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    static const Interval EMPTY, UNIVERSE;
};

const Interval Interval::EMPTY    = Interval(+INF, -INF);
const Interval Interval::UNIVERSE = Interval(-INF, +INF);

#endif