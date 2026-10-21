#pragma once

#include <vector>
#include <initializer_list>

/**
 * @brief Represents a 3D vector with x, y, and z coordinates[cite: 189].
 */
class Vector3 {
public:
    double x;
    double y;
    double z;

    /**
     * @brief Constructs a Vector3 object with given coordinates[cite: 189].
     * @param x X coordinate[cite: 189].
     * @param y Y coordinate[cite: 189].
     * @param z Z coordinate[cite: 189].
     */
    Vector3(double x = 0.0, double y = 0.0, double z = 0.0);

    /**
     * @brief Calculates the length (magnitude) of the vector[cite: 189].
     * @return Length of the vector[cite: 189].
     */
    double length() const;

    /**
     * @brief Returns a normalized (unit) vector[cite: 189].
     * @return Normalized Vector3[cite: 189].
     */
    Vector3 normalized() const;

    /**
     * @brief Calculates the dot product with another vector[cite: 189].
     * @param vector Another Vector3[cite: 189].
     * @return Dot product value[cite: 189].
     */
    double dot(const Vector3& vector) const;

    /**
     * @brief Calculates the cross product with another vector[cite: 189].
     * @param vector Another Vector3[cite: 189].
     * @return Resulting Vector3 perpendicular to both[cite: 189].
     */
    Vector3 cross(const Vector3& vector) const;

    /**
     * @brief Adds another vector[cite: 189].
     * @param vector Vector to add[cite: 189].
     * @return Sum Vector3[cite: 189].
     */
    Vector3 operator+(const Vector3& vector) const;

    /**
     * @brief Subtracts another vector[cite: 189].
     * @param vector Vector to subtract[cite: 189].
     * @return Difference Vector3[cite: 189].
     */
    Vector3 operator-(const Vector3& vector) const;

    /**
     * @brief Multiplies the vector by a scalar[cite: 189].
     * @param scalar Scaling value[cite: 189].
     * @return Scaled Vector3[cite: 189].
     */
    Vector3 operator*(double scalar) const;
};

/**
 * @brief Represents a point in 3D space[cite: 189].
 */
class Point3 {
public:
    double x;
    double y;
    double z;

    /**
     * @brief Constructs a Point3 object with given coordinates[cite: 189].
     * @param x X coordinate[cite: 189].
     * @param y Y coordinate[cite: 189].
     * @param z Z coordinate[cite: 189].
     */
    Point3(double x = 0.0, double y = 0.0, double z = 0.0);

    /**
     * @brief Subtracts another point to get a vector between them[cite: 189].
     * @param other Target point[cite: 189].
     * @return Vector3 from other to this point[cite: 189].
     */
    Vector3 operator-(const Point3& other) const;

    /**
     * @brief Translates the point by a vector[cite: 189].
     * @param vector Translation vector[cite: 189].
     * @return New translated Point3[cite: 189].
     */
    Point3 operator+(const Vector3& vector) const;

    /**
     * @brief Translates the point backward by a vector[cite: 189].
     * @param vector Translation vector[cite: 189].
     * @return New translated Point3[cite: 189].
     */
    Point3 operator-(const Vector3& vector) const;
};

/**
 * @brief Represents a 3D line segment defined by two points[cite: 189].
 */
class Segment3 {
public:
    Point3 a;
    Point3 b;

    /**
     * @brief Constructs a Segment3 with two endpoints[cite: 189].
     * @param a Start point[cite: 189].
     * @param b End point[cite: 189].
     */
    Segment3(const Point3& a = Point3(), const Point3& b = Point3());

    /**
     * @brief Calculates the length of the segment[cite: 189].
     * @return Segment length[cite: 189].
     */
    double length() const;

    /**
     * @brief Checks if the segment is degenerate (zero length)[cite: 189].
     * @return True if degenerate, false otherwise[cite: 189].
     */
    bool is_degenerate() const;

    /**
     * @brief Converts the segment into a direction vector[cite: 189].
     * @return Vector3 from start to end point[cite: 189].
     */
    Vector3 to_vector() const;

    /**
     * @brief Finds the midpoint of the segment[cite: 189].
     * @return Point3 at the middle of the segment[cite: 189].
     */
    Point3 midpoint() const;
};

/**
 * @brief Represents a 3D triangle defined by three points[cite: 189].
 */
class Triangle3 {
public:
    Point3 a;
    Point3 b;
    Point3 c;

    /**
     * @brief Constructs a Triangle3 with three vertices[cite: 189].
     * @param a First vertex[cite: 189].
     * @param b Second vertex[cite: 189].
     * @param c Third vertex[cite: 189].
     */
    Triangle3(const Point3& a = Point3(), const Point3& b = Point3(), const Point3& c = Point3());

    /**
     * @brief Calculates the area of the triangle[cite: 189].
     * @return Triangle area[cite: 189].
     */
    double area() const;

    /**
     * @brief Checks if the triangle is degenerate (zero area)[cite: 189].
     * @return True if degenerate, false otherwise[cite: 189].
     */
    bool is_degenerate() const;

    /**
     * @brief Returns the vector for edge AB[cite: 189].
     * @return Vector3 from a to b[cite: 189].
     */
    Vector3 edge_AB() const;

    /**
     * @brief Returns the vector for edge AC[cite: 189].
     * @return Vector3 from a to c[cite: 189].
     */
    Vector3 edge_AC() const;

    /**
     * @brief Calculates the unit normal vector of the triangle[cite: 189].
     * @return Normalized normal Vector3[cite: 189].
     */
    Vector3 normal() const;
};

/**
 * @brief Represents a 3D polygon with an arbitrary number of vertices[cite: 189].
 */
class Polygon3 {
public:
    unsigned int dots_amount;
    Point3* dot_list;

    /**
     * @brief Default constructor creating an empty polygon[cite: 189].
     */
    Polygon3();

    /**
     * @brief Constructs a polygon from an array of points and size[cite: 189].
     * @param dots_amount Number of points[cite: 189].
     * @param dot_list Pointer to array of Point3[cite: 189].
     */
    Polygon3(unsigned int dots_amount, const Point3* dot_list);

    /**
     * @brief Constructs a polygon from a std::vector of points[cite: 189].
     * @param points Vector of Point3[cite: 189].
     */
    Polygon3(const std::vector<Point3>& points);

    /**
     * @brief Constructs a polygon from an initializer list of points[cite: 189].
     * @param init Initializer list of Point3[cite: 189].
     */
    Polygon3(std::initializer_list<Point3> init);

    /**
     * @brief Destructor freeing dynamically allocated memory[cite: 189].
     */
    ~Polygon3();

    /**
     * @brief Copy constructor for deep copying[cite: 189].
     * @param other Polygon3 to copy from[cite: 189].
     */
    Polygon3(const Polygon3& other);

    /**
     * @brief Copy assignment operator[cite: 189].
     * @param other Polygon3 to assign from[cite: 189].
     * @return Reference to this polygon[cite: 189].
     */
    Polygon3& operator=(const Polygon3& other);

    /**
     * @brief Move constructor[cite: 189].
     * @param other Polygon3 to move from[cite: 189].
     */
    Polygon3(Polygon3&& other) noexcept;

    /**
     * @brief Move assignment operator[cite: 189].
     * @param other Polygon3 to move from[cite: 189].
     * @return Reference to this polygon[cite: 189].
     */
    Polygon3& operator=(Polygon3&& other) noexcept;

    /**
     * @brief Calculates the area of the polygon[cite: 189].
     * @return Polygon area[cite: 189].
     */
    double area() const;

    /**
     * @brief Checks if the polygon is degenerate[cite: 189].
     * @return True if degenerate, false otherwise[cite: 189].
     */
    bool is_degenerate() const;
};
