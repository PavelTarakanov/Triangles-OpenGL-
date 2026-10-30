#include <gtest/gtest.h>
#include "../include/polygon.hpp"
#include "../include/point.hpp"

TEST(Polygon3Test, InitializerListConstructorAndArea) {
    // Assign
    constexpr double epsilon = 1e-6;

    // Act (Квадрат 1x1 на плоскости XY)
    Polygon3 poly = {Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(1.0, 1.0, 0.0), Point3(0.0, 1.0, 0.0)};

    // Assert
    EXPECT_EQ(poly.dots_amount(), 4u);
    EXPECT_NEAR(poly.area(), 1.0, epsilon);
    EXPECT_FALSE(poly.is_degenerate());
}

TEST(Polygon3Test, CopyAndMoveSemantics) {
    // Assign
    Polygon3 orig = {Point3(0.0, 0.0, 0.0), Point3(2.0, 0.0, 0.0), Point3(0.0, 2.0, 0.0)};

    // Act: Copy
    Polygon3 copy = orig;

    // Assert Copy
    EXPECT_EQ(copy.dots_amount(), 3u);
    EXPECT_DOUBLE_EQ(copy.area(), 2.0);

    // Act: Move
    Polygon3 moved = std::move(orig);

    // Assert Move
    EXPECT_EQ(moved.dots_amount(), 3u);
    EXPECT_DOUBLE_EQ(moved.area(), 2.0);
    EXPECT_EQ(orig.dots_amount(), 0u);
    EXPECT_EQ(orig.dot_list(), nullptr);
}
