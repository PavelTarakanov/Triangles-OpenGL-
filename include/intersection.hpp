#pragma once
#include "geometry.hpp"


/**
 * @brief Checks if two triangles lie in the same 3D plane[cite: 3].
 * @param t1 First triangle[cite: 3].
 * @param t2 Second triangle[cite: 3].
 * @return True if they are coplanar, false otherwise[cite: 3].
 */
bool are_coplanar(const Triangle3& t1, const Triangle3& t2);

/**
 * @brief Finds the overlapping polygon area of two coplanar triangles[cite: 3].
 * @param t1 First triangle[cite: 3].
 * @param t2 Second triangle[cite: 3].
 * @return Polygon3 representing the intersection area[cite: 3].
 */
Polygon3 intersect_coplanar_triangles(const Triangle3& t1, const Triangle3& t2);

/**
 * @brief Finds the intersection segment of two non-coplanar triangles.
 * @param t1 First triangle.
 * @param t2 Second triangle.
 * @return Segment3 representing the intersection line segment.
 */
Segment3 intersect_non_coplanar_triangles(const Triangle3& t1, const Triangle3& t2);
