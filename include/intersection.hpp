#pragma once
#include "geometry.hpp"
#include <vector>

/**
 * @brief Checks if two triangles lie in the same 3D plane[cite: 188].
 * @param t1 First triangle[cite: 188].
 * @param t2 Second triangle[cite: 188].
 * @return True if they are coplanar, false otherwise[cite: 188].
 */
bool are_coplanar(const Triangle3& t1, const Triangle3& t2);

/**
 * @brief Finds the overlapping polygon area of two coplanar triangles[cite: 188].
 * @param t1 First triangle[cite: 188].
 * @param t2 Second triangle[cite: 188].
 * @return Polygon3 representing the intersection area[cite: 188].
 */
Polygon3 intersect_coplanar_triangles(const Triangle3& t1, const Triangle3& t2);
