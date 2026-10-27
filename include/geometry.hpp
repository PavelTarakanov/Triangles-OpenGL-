#pragma once

#include <vector>
#include <initializer_list>

class Vector3 {
public:
    double x;
    double y;
    double z;

    Vector3(double x = 0.0, double y = 0.0, double z = 0.0);

    double length() const;
    Vector3 normalized() const;

    double dot(const Vector3& vector) const;

    Vector3 cross(const Vector3& vector) const;
    Vector3 operator+(const Vector3& vector) const;
    Vector3 operator-(const Vector3& vector) const;
    Vector3 operator*(double scalar) const;
};

class Point3 {
public:
    double x;
    double y;
    double z;

    Point3(double x = 0.0, double y = 0.0, double z = 0.0);

    Vector3 operator-(const Point3& other) const;
    Point3 operator+(const Vector3& vector) const;
    Point3 operator-(const Vector3& vector) const;
};

class Segment3 {
public:
    Point3 a;
    Point3 b;

    Segment3(const Point3& a = Point3(), const Point3& b = Point3());

    double length() const;
    bool is_degenerate() const;
    Vector3 to_vector() const;
    Point3 midpoint() const;
};

class Triangle3 {
public:
    Point3 a;
    Point3 b;
    Point3 c;

    Triangle3(const Point3& a = Point3(), const Point3& b = Point3(), const Point3& c = Point3());

    double area() const;

    bool is_degenerate() const;
    Vector3 edge_AB() const;
    Vector3 edge_AC() const;

    Vector3 normal() const;
};

class Polygon3 {
public:
    unsigned int dots_amount;
    Point3* dot_list;

    Polygon3();

    Polygon3(unsigned int dots_amount, const Point3* dot_list);
    Polygon3(const std::vector<Point3>& points);
    Polygon3(std::initializer_list<Point3> init);

    ~Polygon3();
    Polygon3(const Polygon3& other);

    Polygon3& operator=(const Polygon3& other);

    Polygon3(Polygon3&& other) noexcept;
    Polygon3& operator=(Polygon3&& other) noexcept;

    double area() const;

    bool is_degenerate() const;
};
