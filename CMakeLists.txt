cmake_minimum_required(VERSION 3.14)
project(TrianglesIntersection CXX)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing support
enable_testing()

# Find required graphics libraries (OpenGL and GLUT)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Specify header search directories
include_directories(
    include
    src
    .
)

# --- Main Application ---
add_executable(out
    src/main.cpp
    src/intersection.cpp
    src/render.cpp
)

# Link OpenGL and GLUT
target_link_libraries(out PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# --- Unit Tests (Google Test) ---
find_package(GTest REQUIRED)

add_executable(triangles_tests
    tests/vector_tests.cpp
    tests/point_tests.cpp
    tests/segment_tests.cpp
    tests/triangle_tests.cpp
    tests/polygon_tests.cpp
)

target_link_libraries(triangles_tests PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME GeometryUnitTests COMMAND triangles_tests)

# --- End-to-End Tests ---
add_executable(e2e_tests
    tests/e2e_tests.cpp
)

target_link_libraries(e2e_tests PRIVATE GTest::gtest GTest::gtest_main)
add_dependencies(e2e_tests out)

add_test(
    NAME EndToEndTests
    COMMAND e2e_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
