#pragma once
#include "point.hpp"
#include "vector.hpp"
#include <algorithm>

class Triangle3 {
private:
    Point3 a_;
    Point3 b_;
    Point3 c_;
    Point3 max_dot_;
    Point3 min_dot_;

    void update_bounds() {
        max_dot_ = Point3(
            std::max({a_.x(), b_.x(), c_.x()}),
            std::max({a_.y(), b_.y(), c_.y()}),
            std::max({a_.z(), b_.z(), c_.z()})
        );
        min_dot_ = Point3(
            std::min({a_.x(), b_.x(), c_.x()}),
            std::min({a_.y(), b_.y(), c_.y()}),
            std::min({a_.z(), b_.z(), c_.z()})
        );
    }

public:
    Triangle3(const Point3& a = Point3(), const Point3& b = Point3(), const Point3& c = Point3())
        : a_(a), b_(b), c_(c) {
        update_bounds();
    }

    const Point3& a() const { return a_; }
    const Point3& b() const { return b_; }
    const Point3& c() const { return c_; }
    const Point3& Max_dot() const { return max_dot_; }
    const Point3& Min_dot() const { return min_dot_; }

    bool intersects_aabb(const Triangle3& other) const {
        return (Min_dot().x() <= other.Max_dot().x() && Max_dot().x() >= other.Min_dot().x()) &&
               (Min_dot().y() <= other.Max_dot().y() && Max_dot().y() >= other.Min_dot().y()) &&
               (Min_dot().z() <= other.Max_dot().z() && Max_dot().z() >= other.Min_dot().z());
    }

    Vector3 edge_AB() const {
        return b_ - a_;
    }

    Vector3 edge_AC() const {
        return c_ - a_;
    }

    double area() const {
        return edge_AB().cross(edge_AC()).length() / 2.0;
    }

    bool is_degenerate() const {
        return area() < std::numeric_limits<double>::epsilon();
    }

    Vector3 normal() const {
        return edge_AB().cross(edge_AC()).normalized();
    }

    void find_max_and_min() {
        update_bounds();
    }
};
