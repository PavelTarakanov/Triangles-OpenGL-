#include <iostream>
#include <fstream>
#include <string>
#include "../include/point.hpp"
#include "../include/segment.hpp"
#include "../include/triangle.hpp"
#include "../include/polygon.hpp"
#include "../include/intersection.hpp"
#include "../include/render.hpp"

int main(int argc, char** argv) {
    std::istream* input_stream = &std::cin;
    std::ifstream file_stream;
    bool headless = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--headless") {
            headless = true;
        } else {
            file_stream.open(arg);
            if (file_stream.is_open()) {
                input_stream = &file_stream;
            } else {
                std::cerr << "Error: Could not open file " << arg << std::endl;
                return 1;
            }
        }
    }

    int n;
    if (!(*input_stream >> n) || n < 0) {
        std::cerr << "Error: Invalid triangle count" << std::endl;
        return 1;
    }

    Scene scene;

    for (int i = 0; i < n; ++i) {
        Point3 p1, p2, p3;
        if (!(*input_stream >> p1.x() >> p1.y() >> p1.z()
                            >> p2.x() >> p2.y() >> p2.z()
                            >> p3.x() >> p3.y() >> p3.z())) {
            std::cerr << "Error: Incomplete vertex data" << std::endl;
            return 1;
        }
        scene.triangles.push_back(Triangle3(p1, p2, p3));
    }

    // Broad-phase AABB + Narrow-phase intersection detection
    for (size_t i = 0; i < scene.triangles.size(); ++i) {
        for (size_t j = i + 1; j < scene.triangles.size(); ++j) {
            if (!scene.triangles[i].intersects_aabb(scene.triangles[j])) {
                continue;
            }

            if (are_coplanar(scene.triangles[i], scene.triangles[j])) {
                Polygon3 poly = intersect_coplanar_triangles(scene.triangles[i], scene.triangles[j]);
                if (poly.dots_amount() > 2) {
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

    if (headless) {
        std::cout << "TRIANGLES: " << scene.triangles.size() << "\n";
        std::cout << "COPLANAR_POLYGONS: " << scene.intersections.size() << "\n";
        std::cout << "NON_COPLANAR_SEGMENTS: " << scene.intersection_segments.size() << "\n";
        for (const auto& seg : scene.intersection_segments) {
            std::cout << "SEGMENT: ("
                      << seg.a().x() << ", " << seg.a().y() << ", " << seg.a().z() << ") -> ("
                      << seg.b().x() << ", " << seg.b().y() << ", " << seg.b().z() << ")\n";
        }
        return 0;
    }

    start_render(argc, argv, scene);
    return 0;
}
