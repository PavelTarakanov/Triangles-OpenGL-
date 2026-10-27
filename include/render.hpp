#pragma once
#include "segment.hpp"
#include "triangle.hpp"
#include "polygon.hpp"

struct Scene {
    std::vector<Triangle3> triangles;
    std::vector<Polygon3> intersections;
    std::vector<Segment3> intersection_segments;
};

void start_render(int argc, char** argv, const Scene& scene);
