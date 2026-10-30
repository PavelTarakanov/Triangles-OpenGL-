#include <gtest/gtest.h>
#include "../include/point.hpp"
#include "../include/vector.hpp"

TEST(Point3Test, Constructor) {
    // Assign & Act
    Point3 point(1.0, 2.0, 3.0);

    // Assert
    EXPECT_DOUBLE_EQ(point.x(), 1.0);
    EXPECT_DOUBLE_EQ(point.y(), 2.0);
    EXPECT_DOUBLE_EQ(point.z(), 3.0);
}

TEST(Point3Test, PointMinusPoint) {
    // Assign
    Point3 a(4.0, 5.0, 6.0);
    Point3 b(1.0, 2.0, 3.0);

    // Act
    Vector3 result = a - b;

    // Assert
    EXPECT_DOUBLE_EQ(result.x(), 3.0);
    EXPECT_DOUBLE_EQ(result.y(), 3.0);
    EXPECT_DOUBLE_EQ(result.z(), 3.0);
}

TEST(Point3Test, PointPlusVector) {
    // Assign
    Point3 point(1.0, 2.0, 3.0);
    Vector3 vector(4.0, 5.0, 6.0);

    // Act
    Point3 result = point + vector;

    // Assert
    EXPECT_DOUBLE_EQ(result.x(), 5.0);
    EXPECT_DOUBLE_EQ(result.y(), 7.0);
    EXPECT_DOUBLE_EQ(result.z(), 9.0);
}

TEST(Point3Test, PointMinusVector) {
    // Assign
    Point3 point(4.0, 5.0, 6.0);
    Vector3 vector(1.0, 2.0, 3.0);

    // Act
    Point3 result = point - vector;

    // Assert
    EXPECT_DOUBLE_EQ(result.x(), 3.0);
    EXPECT_DOUBLE_EQ(result.y(), 3.0);
    EXPECT_DOUBLE_EQ(result.z(), 3.0);
}
