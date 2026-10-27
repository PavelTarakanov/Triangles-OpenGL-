#pragma once
#include "geometry.hpp"
#include <vector>

struct Scene {
    std::vector<Triangle3> triangles;
    std::vector<Polygon3> intersections;
    std::vector<Segment3> intersection_segments;
};

void start_render(int argc, char** argv, const Scene& scene);
