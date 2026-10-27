#pragma once
#include "point.hpp"
#include <vector>
#include <initializer_list>
#include <limits>

class Polygon3 {
private:
    unsigned int dots_amount_;
    Point3* dot_list_;

public:
    Polygon3() : dots_amount_(0), dot_list_(nullptr) {}

    Polygon3(unsigned int dots_amount, const Point3* dot_list)
        : dots_amount_(dots_amount), dot_list_(nullptr) {
        if (dots_amount_ > 0 && dot_list != nullptr) {
            dot_list_ = new Point3[dots_amount_];
            for (unsigned int i = 0; i < dots_amount_; ++i) {
                dot_list_[i] = dot_list[i];
            }
        }
    }

    Polygon3(const std::vector<Point3>& points)
        : dots_amount_(static_cast<unsigned int>(points.size())), dot_list_(nullptr) {
        if (dots_amount_ > 0) {
            dot_list_ = new Point3[dots_amount_];
            for (unsigned int i = 0; i < dots_amount_; ++i) {
                dot_list_[i] = points[i];
            }
        }
    }

    Polygon3(std::initializer_list<Point3> init)
        : dots_amount_(static_cast<unsigned int>(init.size())), dot_list_(nullptr) {
        if (dots_amount_ > 0) {
            dot_list_ = new Point3[dots_amount_];
            unsigned int i = 0;
            for (const auto& pt : init) {
                dot_list_[i++] = pt;
            }
        }
    }

    ~Polygon3() {
        delete[] dot_list_;
    }

    Polygon3(const Polygon3& other)
        : dots_amount_(other.dots_amount_), dot_list_(nullptr) {
        if (dots_amount_ > 0 && other.dot_list_ != nullptr) {
            dot_list_ = new Point3[dots_amount_];
            for (unsigned int i = 0; i < dots_amount_; ++i) {
                dot_list_[i] = other.dot_list_[i];
            }
        }
    }

    Polygon3& operator=(const Polygon3& other) {
        if (this != &other) {
            Point3* new_list = nullptr;
            if (other.dots_amount_ > 0 && other.dot_list_ != nullptr) {
                new_list = new Point3[other.dots_amount_];
                for (unsigned int i = 0; i < other.dots_amount_; ++i) {
                    new_list[i] = other.dot_list_[i];
                }
            }
            delete[] dot_list_;
            dot_list_ = new_list;
            dots_amount_ = other.dots_amount_;
        }
        return *this;
    }

    Polygon3(Polygon3&& other) noexcept
        : dots_amount_(other.dots_amount_), dot_list_(other.dot_list_) {
        other.dots_amount_ = 0;
        other.dot_list_ = nullptr;
    }

    Polygon3& operator=(Polygon3&& other) noexcept {
        if (this != &other) {
            delete[] dot_list_;
            dots_amount_ = other.dots_amount_;
            dot_list_ = other.dot_list_;
            other.dots_amount_ = 0;
            other.dot_list_ = nullptr;
        }
        return *this;
    }

    unsigned int dots_amount() const { return dots_amount_; }
    const Point3* dot_list() const { return dot_list_; }

    double area() const {
        if (dots_amount_ < 3 || dot_list_ == nullptr) {
            return 0.0;
        }
        Vector3 total_cross(0.0, 0.0, 0.0);
        Point3 origin = dot_list_[0];
        for (unsigned int i = 1; i + 1 < dots_amount_; ++i) {
            Vector3 v1 = dot_list_[i] - origin;
            Vector3 v2 = dot_list_[i + 1] - origin;
            total_cross = total_cross + v1.cross(v2);
        }
        return total_cross.length() / 2.0;
    }

    bool is_degenerate() const {
        return dots_amount_ < 3 || area() < std::numeric_limits<double>::epsilon();
    }
};
