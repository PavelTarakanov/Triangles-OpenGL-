#include <iostream>
#include <fstream>
#include "..//include/geometry.hpp"
#include "..//include/intersection.hpp"
#include "..//include/render.hpp"

int main(int argc, char** argv) {
    std::istream* input_stream = &std::cin;
    std::ifstream file_stream;

    if (argc > 1) {
        file_stream.open(argv[1]);
        if (file_stream.is_open()) {
            input_stream = &file_stream;
        } else {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
    }

    int n;
    if (!(*input_stream >> n)) return 0;

    Scene scene;

    for (int i = 0; i < n; ++i) {
        Point3 p1, p2, p3;
        *input_stream >> p1.x >> p1.y >> p1.z;
        *input_stream >> p2.x >> p2.y >> p2.z;
        *input_stream >> p3.x >> p3.y >> p3.z;
        scene.triangles.push_back(Triangle3(p1, p2, p3));
    }

    // Find intersections between all pairs of triangles
    for (size_t i = 0; i < scene.triangles.size(); ++i) {
        for (size_t j = i + 1; j < scene.triangles.size(); ++j) {
            if (are_coplanar(scene.triangles[i], scene.triangles[j])) {
                Polygon3 poly = intersect_coplanar_triangles(scene.triangles[i], scene.triangles[j]);
                if (poly.dots_amount > 2) {
                    scene.intersections.push_back(poly);
                }
            } else {
                Segment3 seg = intersect_non_coplanar_triangles(scene.triangles[i], scene.triangles[j]);
                if (!seg.is_degenerate()) {
                    scene.intersection_segments.push_back(seg);
                }
            }
        }
    }

    start_render(argc, argv, scene);
    return 0;
}
