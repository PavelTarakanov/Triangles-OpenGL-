#include <gtest/gtest.h>
#include "../include/triangle.hpp"
#include "../include/point.hpp"
#include "../include/vector.hpp"

TEST(Triangle3Test, Edges) {
    // Assign
    Triangle3 triangle(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.0, 1.0, 0.0));

    // Act
    Vector3 ab = triangle.edge_AB();
    Vector3 ac = triangle.edge_AC();

    // Assert
    EXPECT_DOUBLE_EQ(ab.x(), 1.0);
    EXPECT_DOUBLE_EQ(ab.y(), 0.0);
    EXPECT_DOUBLE_EQ(ab.z(), 0.0);

    EXPECT_DOUBLE_EQ(ac.x(), 0.0);
    EXPECT_DOUBLE_EQ(ac.y(), 1.0);
    EXPECT_DOUBLE_EQ(ac.z(), 0.0);
}

TEST(Triangle3Test, Area) {
    // Assign
    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    Triangle3 triangle(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.0, 1.0, 0.0));

    // Act
    double area = triangle.area();

    // Assert
    EXPECT_NEAR(area, 0.5, epsilon);
}

TEST(Triangle3Test, Normal) {
    // Assign
    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    Triangle3 triangle(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.0, 1.0, 0.0));

    // Act
    Vector3 normal = triangle.normal();

    // Assert
    EXPECT_NEAR(normal.x(), 0.0, epsilon);
    EXPECT_NEAR(normal.y(), 0.0, epsilon);
    EXPECT_NEAR(normal.z(), 1.0, epsilon);
}

TEST(Triangle3Test, NonDegenerate) {
    // Assign
    Triangle3 triangle(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.0, 1.0, 0.0));

    // Act & Assert
    EXPECT_FALSE(triangle.is_degenerate());
}

TEST(Triangle3Test, Degenerate) {
    // Assign
    Triangle3 triangle(Point3(0.0, 0.0, 0.0), Point3(1.0, 1.0, 1.0), Point3(2.0, 2.0, 2.0));

    // Act & Assert
    EXPECT_TRUE(triangle.is_degenerate());
}

TEST(Triangle3Test, AABBIntersection) {
    // Assign
    Triangle3 t1(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.0, 1.0, 0.0));
    Triangle3 t2(Point3(0.5, 0.5, 0.0), Point3(1.5, 0.5, 0.0), Point3(0.5, 1.5, 0.0));
    Triangle3 t3(Point3(5.0, 5.0, 5.0), Point3(6.0, 5.0, 5.0), Point3(5.0, 6.0, 5.0));

    // Act & Assert
    EXPECT_TRUE(t1.intersects_aabb(t2));
    EXPECT_FALSE(t1.intersects_aabb(t3));
}
