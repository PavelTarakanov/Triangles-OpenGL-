#include <gtest/gtest.h>
#include "../include/segment.hpp"
#include "../include/point.hpp"

TEST(Segment3Test, ConstructorAndProperties) {
    // Assign
    Point3 p1(0.0, 0.0, 0.0);
    Point3 p2(3.0, 4.0, 0.0);

    // Act
    Segment3 seg(p1, p2);

    // Assert
    EXPECT_DOUBLE_EQ(seg.length(), 5.0);
    EXPECT_FALSE(seg.is_degenerate());
    EXPECT_DOUBLE_EQ(seg.midpoint().x(), 1.5);
    EXPECT_DOUBLE_EQ(seg.midpoint().y(), 2.0);
    EXPECT_DOUBLE_EQ(seg.midpoint().z(), 0.0);
}

TEST(Segment3Test, DegenerateSegment) {
    // Assign
    Point3 p1(1.0, 1.0, 1.0);
    Point3 p2(1.0, 1.0, 1.0);

    // Act
    Segment3 seg(p1, p2);

    // Assert
    EXPECT_TRUE(seg.is_degenerate());
    EXPECT_DOUBLE_EQ(seg.length(), 0.0);
}
