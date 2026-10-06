#include "geometry.hpp"

//--------------//
//Vector methods//
//--------------//
Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

double Vector3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() const {
    double len = length();
    if (len < std::numeric_limits<double>::epsilon()) {
        return Vector3();
    }
    return Vector3(x / len, y / len, z / len);
}

double Vector3::dot(const Vector3& vector) const {
    return x * vector.x + y * vector.y + z * vector.z;
}

Vector3 Vector3::cross(const Vector3& vector) const {
    return Vector3(y * vector.z - z * vector.y,
                   z * vector.x - x * vector.z,
                   x * vector.y - y * vector.x);
}

Vector3 Vector3::operator+(const Vector3& vector) const {
    return Vector3(x + vector.x, y + vector.y, z + vector.z);
}

Vector3 Vector3::operator-(const Vector3& vector) const {
    return Vector3(x - vector.x, y - vector.y, z - vector.z);
}

Vector3 Vector3::operator*(double scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

//-------------//
//Point methods//
//-------------//
Point3::Point3(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3 Point3::operator-(const Point3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Point3 Point3::operator+(const Vector3& vector) const {
    return Point3(x + vector.x, y + vector.y, z + vector.z);
}

Point3 Point3::operator-(const Vector3& vector) const {
    return Point3(x - vector.x, y - vector.y, z - vector.z);
}

//---------------//
//Segment methods//
//---------------//
Segment3::Segment3(const Point3& a, const Point3& b) : a(a), b(b) {}

double Segment3::length() const {
    return (b - a).length();
}

bool Segment3::is_degenerate() const {
    return length() < std::numeric_limits<double>::epsilon();
}

Vector3 Segment3::to_vector() const {
    return b - a;
}

Point3 Segment3::midpoint() const {
    return a + to_vector() * 0.5;
}

//----------------//
//Triangle methods//
//----------------//
Triangle3::Triangle3(const Point3& a, const Point3& b, const Point3& c) : a(a), b(b), c(c) {}

double Triangle3::area() const {
    return edge_AB().cross(edge_AC()).length() / 2.0;
}

bool Triangle3::is_degenerate() const {
    return area() < std::numeric_limits<double>::epsilon();
}

Vector3 Triangle3::edge_AB() const {
    return b - a;
}

Vector3 Triangle3::edge_AC() const {
    return c - a;
}

Vector3 Triangle3::normal() const {
    return edge_AB().cross(edge_AC()).normalized();
}

//----------------//
//Polygon methods //
//----------------//
Polygon3::Polygon3() : dots_amount(0), dot_list(nullptr) {}

Polygon3::Polygon3(unsigned int dots_amount, const Point3* dot_list)
    : dots_amount(dots_amount), dot_list(nullptr) {
    if (dots_amount > 0 && dot_list != nullptr) {
        this->dot_list = new Point3[dots_amount];
        for (unsigned int i = 0; i < dots_amount; ++i) {
            this->dot_list[i] = dot_list[i];
        }
    }
}

Polygon3::Polygon3(const std::vector<Point3>& points)
    : dots_amount(static_cast<unsigned int>(points.size())), dot_list(nullptr) {
    if (dots_amount > 0) {
        dot_list = new Point3[dots_amount];
        for (unsigned int i = 0; i < dots_amount; ++i) {
            dot_list[i] = points[i];
        }
    }
}

Polygon3::Polygon3(std::initializer_list<Point3> init)
    : dots_amount(static_cast<unsigned int>(init.size())), dot_list(nullptr) {
    if (dots_amount > 0) {
        dot_list = new Point3[dots_amount];
        unsigned int i = 0;
        for (const auto& pt : init) {
            dot_list[i++] = pt;
        }
    }
}

Polygon3::~Polygon3() {
    delete[] dot_list;
}

Polygon3::Polygon3(const Polygon3& other)
    : dots_amount(other.dots_amount), dot_list(nullptr) {
    if (dots_amount > 0 && other.dot_list != nullptr) {
        dot_list = new Point3[dots_amount];
        for (unsigned int i = 0; i < dots_amount; ++i) {
            dot_list[i] = other.dot_list[i];
        }
    }
}

Polygon3& Polygon3::operator=(const Polygon3& other) {
    if (this != &other) {
        Point3* new_list = nullptr;
        if (other.dots_amount > 0 && other.dot_list != nullptr) {
            new_list = new Point3[other.dots_amount];
            for (unsigned int i = 0; i < other.dots_amount; ++i) {
                new_list[i] = other.dot_list[i];
            }
        }
        delete[] dot_list;
        dot_list = new_list;
        dots_amount = other.dots_amount;
    }
    return *this;
}

Polygon3::Polygon3(Polygon3&& other) noexcept
    : dots_amount(other.dots_amount), dot_list(other.dot_list) {
    other.dots_amount = 0;
    other.dot_list = nullptr;
}

Polygon3& Polygon3::operator=(Polygon3&& other) noexcept {
    if (this != &other) {
        delete[] dot_list;
        dots_amount = other.dots_amount;
        dot_list = other.dot_list;
        other.dots_amount = 0;
        other.dot_list = nullptr;
    }
    return *this;
}

double Polygon3::area() const {
    if (dots_amount < 3 || dot_list == nullptr) {
        return 0.0;
    }
    Vector3 total_cross(0.0, 0.0, 0.0);
    Point3 origin = dot_list[0];
    for (unsigned int i = 1; i + 1 < dots_amount; ++i) {
        Vector3 v1 = dot_list[i] - origin;
        Vector3 v2 = dot_list[i + 1] - origin;
        total_cross = total_cross + v1.cross(v2);
    }
    return total_cross.length() / 2.0;
}

bool Polygon3::is_degenerate() const {
    return dots_amount < 3 || area() < std::numeric_limits<double>::epsilon();
}
