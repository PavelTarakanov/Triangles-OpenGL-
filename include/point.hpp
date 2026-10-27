#pragma once
#include "vector.hpp"

class Point3 {
private:
    double x_;
    double y_;
    double z_;

public:
    Point3(double x = 0.0, double y = 0.0, double z = 0.0) : x_(x), y_(y), z_(z) {}

    double x() const { return x_; }
    double& x() { return x_; }
    double y() const { return y_; }
    double& y() { return y_; }
    double z() const { return z_; }
    double& z() { return z_; }

    Vector3 operator-(const Point3& other) const {
        return Vector3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }

    Point3 operator+(const Vector3& vector) const {
        return Point3(x_ + vector.x(), y_ + vector.y(), z_ + vector.z());
    }

    Point3 operator-(const Vector3& vector) const {
        return Point3(x_ - vector.x(), y_ - vector.y(), z_ - vector.z());
    }
};
