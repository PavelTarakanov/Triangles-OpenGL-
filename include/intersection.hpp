#pragma once
#include "segment.hpp"
#include "triangle.hpp"
#include "polygon.hpp"

bool are_coplanar(const Triangle3& t1, const Triangle3& t2);

Polygon3 intersect_coplanar_triangles(const Triangle3& t1, const Triangle3& t2);

Segment3 intersect_non_coplanar_triangles(const Triangle3& t1, const Triangle3& t2);
