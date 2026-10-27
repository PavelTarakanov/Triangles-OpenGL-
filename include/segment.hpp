#pragma once
#include "point.hpp"
#include "vector.hpp"

class Segment3 {
private:
    Point3 a_;
    Point3 b_;

public:
    Segment3(const Point3& a = Point3(), const Point3& b = Point3()) : a_(a), b_(b) {}

    const Point3& a() const { return a_; }
    Point3& a() { return a_; }
    const Point3& b() const { return b_; }
    Point3& b() { return b_; }

    double length() const {
        return (b_ - a_).length();
    }

    bool is_degenerate() const {
        return length() < std::numeric_limits<double>::epsilon();
    }

    Vector3 to_vector() const {
        return b_ - a_;
    }

    Point3 midpoint() const {
        return a_ + to_vector() * 0.5;
    }
};
