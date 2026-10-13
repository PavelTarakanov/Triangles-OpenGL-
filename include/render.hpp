#pragma once
#include "geometry.hpp"
#include <vector>

/**
 * @brief Holds the 3D scene data containing triangles and their intersections[cite: 187].
 */
struct Scene {
    std::vector<Triangle3> triangles;
    std::vector<Polygon3> intersections;
};

/**
 * @brief Initializes and starts the OpenGL rendering loop[cite: 187].
 * @param argc Argument count[cite: 187].
 * @param argv Argument values[cite: 187].
 * @param scene Scene data to render[cite: 187].
 */
void start_render(int argc, char** argv, const Scene& scene);
