#include <iostream>
#include "geometry.hpp"
#include "intersection.hpp"
#include "render.hpp"

/**
 * @brief Main entry point of the program[cite: 184].
 * @param argc Argument count[cite: 184].
 * @param argv Argument values[cite: 184].
 * @return Exit status code[cite: 184].
 */
int main(int argc, char** argv) {
    int n;
    if (!(std::cin >> n)) return 0;

    Scene scene;

    for (int i = 0; i < n; ++i) {
        Point3 p1, p2, p3;
        std::cin >> p1.x >> p1.y >> p1.z;
        std::cin >> p2.x >> p2.y >> p2.z;
        std::cin >> p3.x >> p3.y >> p3.z;
        scene.triangles.push_back(Triangle3(p1, p2, p3));
    }

    for (size_t i = 0; i < scene.triangles.size(); ++i) {
        for (size_t j = i + 1; j < scene.triangles.size(); ++j) {
            Polygon3 poly = intersect_coplanar_triangles(scene.triangles[i],
                                                         scene.triangles[j]);
            if (poly.dots_amount > 2) {
                scene.intersections.push_back(poly);
            }
        }
    }

    start_render(argc, argv, scene);
    return 0;
}
