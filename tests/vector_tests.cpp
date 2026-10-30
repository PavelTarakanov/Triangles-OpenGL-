#include <gtest/gtest.h>
#include "../include/vector.hpp"

TEST(Vector3Test, Constructor) {
    // Assign & Act
    Vector3 vector(1.0, 2.0, 3.0);

    // Assert
    EXPECT_DOUBLE_EQ(vector.x(), 1.0);
    EXPECT_DOUBLE_EQ(vector.y(), 2.0);
    EXPECT_DOUBLE_EQ(vector.z(), 3.0);
}

TEST(Vector3Test, DefaultConstructor) {
    // Assign & Act
    Vector3 vector;

    // Assert
    EXPECT_DOUBLE_EQ(vector.x(), 0.0);
    EXPECT_DOUBLE_EQ(vector.y(), 0.0);
    EXPECT_DOUBLE_EQ(vector.z(), 0.0);
}

TEST(Vector3Test, Addition) {
    // Assign
    Vector3 a(1.0, 2.0, 3.0);
    Vector3 b(4.0, 5.0, 6.0);

    // Act
    Vector3 result = a + b;

    // Assert
    EXPECT_DOUBLE_EQ(result.x(), 5.0);
    EXPECT_DOUBLE_EQ(result.y(), 7.0);
    EXPECT_DOUBLE_EQ(result.z(), 9.0);
}

TEST(Vector3Test, Subtraction) {
    // Assign
    Vector3 a(4.0, 5.0, 6.0);
    Vector3 b(1.0, 2.0, 3.0);

    // Act
    Vector3 result = a - b;

    // Assert
    EXPECT_DOUBLE_EQ(result.x(), 3.0);
    EXPECT_DOUBLE_EQ(result.y(), 3.0);
    EXPECT_DOUBLE_EQ(result.z(), 3.0);
}

TEST(Vector3Test, ScalarMultiplication) {
    // Assign
    Vector3 vector(1.0, 2.0, 3.0);

    // Act
    Vector3 result = vector * 2.0;

    // Assert
    EXPECT_DOUBLE_EQ(result.x(), 2.0);
    EXPECT_DOUBLE_EQ(result.y(), 4.0);
    EXPECT_DOUBLE_EQ(result.z(), 6.0);
}

TEST(Vector3Test, Length) {
    // Assign
    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    Vector3 vector(3.0, 4.0, 0.0);

    // Act
    double len = vector.length();

    // Assert
    EXPECT_NEAR(len, 5.0, epsilon);
}

TEST(Vector3Test, Normalized) {
    // Assign
    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    Vector3 vector(3.0, 4.0, 0.0);

    // Act
    Vector3 result = vector.normalized();

    // Assert
    EXPECT_NEAR(result.x(), 0.6, epsilon);
    EXPECT_NEAR(result.y(), 0.8, epsilon);
    EXPECT_NEAR(result.z(), 0.0, epsilon);
    EXPECT_NEAR(result.length(), 1.0, epsilon);
}

TEST(Vector3Test, DotProduct) {
    // Assign
    Vector3 a(1.0, 2.0, 3.0);
    Vector3 b(4.0, 5.0, 6.0);

    // Act
    double dot_prod = a.dot(b);

    // Assert
    EXPECT_DOUBLE_EQ(dot_prod, 32.0);
}

TEST(Vector3Test, CrossProduct) {
    // Assign
    Vector3 a(1.0, 0.0, 0.0);
    Vector3 b(0.0, 1.0, 0.0);

    // Act
    Vector3 result = a.cross(b);

    // Assert
    EXPECT_DOUBLE_EQ(result.x(), 0.0);
    EXPECT_DOUBLE_EQ(result.y(), 0.0);
    EXPECT_DOUBLE_EQ(result.z(), 1.0);
}
