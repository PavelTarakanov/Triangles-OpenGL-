#pragma once
#include "geometry.hpp"
#include <vector>

/**
 * @brief Holds the 3D scene data containing triangles, intersection polygons and segments[cite: 1].
 */
struct Scene {
    std::vector<Triangle3> triangles;
    std::vector<Polygon3> intersections;
    std::vector<Segment3> intersection_segments;
};

/**
 * @brief Initializes and starts the OpenGL rendering loop[cite: 1].
 * @param argc Argument count[cite: 1].
 * @param argv Argument values[cite: 1].
 * @param scene Scene data to render[cite: 1].
 */
void start_render(int argc, char** argv, const Scene& scene);
