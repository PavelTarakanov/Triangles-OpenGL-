cmake_minimum_required(VERSION 3.14)
project(TrianglesIntersection CXX)

# Используем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем поддержку тестирования
enable_testing()

# Поиск необходимых графических библиотек (OpenGL и GLUT)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Указываем пути к заголовочным файлам
include_directories(
    include
    src
    .
)

# --- Основное приложение ---
add_executable(out
    src/main.cpp
    src/intersection.cpp
    src/render.cpp
)

# Линковка OpenGL и GLUT (работает кроссплатформенно на macOS и Linux)
target_link_libraries(out PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# --- Юнит-тесты (Google Test) ---
find_package(GTest REQUIRED)

add_executable(triangles_tests
    tests/geometry_tests.cpp
)

# Линковка Google Test
target_link_libraries(triangles_tests PRIVATE GTest::gtest GTest::gtest_main)

# Регистрация теста для CTest
add_test(NAME GeometryUnitTests COMMAND triangles_tests)
