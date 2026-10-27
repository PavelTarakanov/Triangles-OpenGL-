#pragma once
#include <cmath>
#include <limits>

class Vector3 {
private:
    double x_;
    double y_;
    double z_;

public:
    Vector3(double x = 0.0, double y = 0.0, double z = 0.0) : x_(x), y_(y), z_(z) {}

    double x() const { return x_; }
    double& x() { return x_; }
    double y() const { return y_; }
    double& y() { return y_; }
    double z() const { return z_; }
    double& z() { return z_; }

    double length() const {
        return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    }

    Vector3 normalized() const {
        double len = length();
        if (len < std::numeric_limits<double>::epsilon()) {
            return Vector3();
        }
        return Vector3(x_ / len, y_ / len, z_ / len);
    }

    double dot(const Vector3& vector) const {
        return x_ * vector.x_ + y_ * vector.y_ + z_ * vector.z_;
    }

    Vector3 cross(const Vector3& vector) const {
        return Vector3(y_ * vector.z_ - z_ * vector.y_,
                       z_ * vector.x_ - x_ * vector.z_,
                       x_ * vector.y_ - y_ * vector.x_);
    }

    Vector3 operator+(const Vector3& vector) const {
        return Vector3(x_ + vector.x_, y_ + vector.y_, z_ + vector.z_);
    }

    Vector3 operator-(const Vector3& vector) const {
        return Vector3(x_ - vector.x_, y_ - vector.y_, z_ - vector.z_);
    }

    Vector3 operator*(double scalar) const {
        return Vector3(x_ * scalar, y_ * scalar, z_ * scalar);
    }
};
