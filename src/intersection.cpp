#include <cmath>
#include <vector>
#include <algorithm>
#include "..//include/intersection.hpp"

constexpr double EPS = 1e-6;

bool are_coplanar(const Triangle3& t1, const Triangle3& t2) {
    Vector3 n1 = t1.normal();
    Vector3 n2 = t2.normal();
    if (n1.cross(n2).length() > EPS) return false;
    double dist = std::abs(n1.dot(t2.a - t1.a));
    return dist < EPS;
}

struct Point2D { double x, y; };

/**
 * @brief Computes the 2D cross product (oriented area) of three points.
 * @param a First point.
 * @param b Second point.
 * @param c Third point.
 * @return Double value representing the oriented side.
 */
double cross2d(Point2D a, Point2D b, Point2D c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

/**
 * @brief Finds the intersection point of two 2D lines using Cramer's rule.
 * @param a Start point of the first line.
 * @param b End point of the first line.
 * @param p Start point of the second line.
 * @param q End point of the second line.
 * @return Intersection Point2D.
 */
Point2D intersect_lines2d(Point2D a, Point2D b, Point2D p, Point2D q) {
    double A1 = b.y - a.y, B1 = a.x - b.x, C1 = A1 * a.x + B1 * a.y;
    double A2 = q.y - p.y, B2 = p.x - q.x, C2 = A2 * p.x + B2 * p.y;
    double det = A1 * B2 - A2 * B1;
    if (std::abs(det) < EPS) return a;
    return {(B2 * C1 - B1 * C2) / det, (A1 * C2 - A2 * C1) / det};
}

Polygon3 intersect_coplanar_triangles(const Triangle3& t1, const Triangle3& t2) {
    if (t1.is_degenerate() || t2.is_degenerate()) return Polygon3();
    if (!are_coplanar(t1, t2)) return Polygon3();

    Vector3 n = t1.normal();
    int drop_axis = 0;
    if (std::abs(n.y) > std::abs(n.x)) drop_axis = 1;
    if (std::abs(n.z) > std::abs(n.x) && std::abs(n.z) > std::abs(n.y)) drop_axis = 2;

    auto to2d = [&](const Point3& p) -> Point2D {
        if (drop_axis == 0) return {p.y, p.z};
        if (drop_axis == 1) return {p.x, p.z};
        return {p.x, p.y};
    };

    auto to3d = [&](const Point2D& p2, const Point3& ref) -> Point3 {
        double A = n.x, B = n.y, C = n.z;
        double D = -(A * ref.x + B * ref.y + C * ref.z);
        if (drop_axis == 0) return {-(B * p2.x + C * p2.y + D) / A, p2.x, p2.y};
        if (drop_axis == 1) return {p2.x, -(A * p2.x + C * p2.y + D) / B, p2.y};
        return {p2.x, p2.y, -(A * p2.x + B * p2.y + D) / C};
    };

    std::vector<Point2D> poly1 = {to2d(t1.a), to2d(t1.b), to2d(t1.c)};
    std::vector<Point2D> poly2 = {to2d(t2.a), to2d(t2.b), to2d(t2.c)};

    if (cross2d(poly2[0], poly2[1], poly2[2]) < 0) {
        std::reverse(poly2.begin(), poly2.end());
    }

    std::vector<Point2D> clipped = poly1;
    for (int i = 0; i < 3; ++i) {
        Point2D edge_start = poly2[i];
        Point2D edge_end = poly2[(i + 1) % 3];
        std::vector<Point2D> new_clipped;

        for (size_t j = 0; j < clipped.size(); ++j) {
            Point2D p_curr = clipped[j];
            Point2D p_prev = clipped[(j + clipped.size() - 1) % clipped.size()];

            bool curr_inside = cross2d(edge_start, edge_end, p_curr) >= -EPS;
            bool prev_inside = cross2d(edge_start, edge_end, p_prev) >= -EPS;

            if (curr_inside != prev_inside) {
                new_clipped.push_back(intersect_lines2d(p_prev, p_curr, edge_start, edge_end));
            }
            if (curr_inside) {
                new_clipped.push_back(p_curr);
            }
        }
        clipped = new_clipped;
        if (clipped.empty()) break;
    }

    std::vector<Point3> res3d;
    for (auto& p2 : clipped) res3d.push_back(to3d(p2, t1.a));
    return Polygon3(res3d);
}

/**
 * @brief Checks if a 3D point is inside a triangle.
 */
bool point_in_triangle(const Point3& p, const Triangle3& t) {
    Vector3 v0 = t.b - t.a;
    Vector3 v1 = t.c - t.a;
    Vector3 v2 = p - t.a;
    double d00 = v0.dot(v0);
    double d01 = v0.dot(v1);
    double d11 = v1.dot(v1);
    double d20 = v2.dot(v0);
    double d21 = v2.dot(v1);
    double denom = d00 * d11 - d01 * d01;
    if (std::abs(denom) < 1e-9) return false;
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;
    const double eps = -1e-5;
    return (u >= eps) && (v >= eps) && (w >= eps) && (u <= 1.0 - eps) && (v <= 1.0 - eps) && (w <= 1.0 - eps);
}

/**
 * @brief Intersects a 3D segment with a triangle's plane.
 */
bool intersect_segment_plane(const Point3& a, const Point3& b, const Triangle3& t, Point3& out_pt) {
    Vector3 n = t.normal();
    // Превращаем Point3 в Vector3 через вычитание с началом координат или через векторные компоненты
    Vector3 ta_vec(t.a.x, t.a.y, t.a.z);
    Vector3 a_vec(a.x, a.y, a.z);
    Vector3 b_vec(b.x, b.y, b.z);

    double d = -n.dot(ta_vec);
    double na = n.dot(a_vec) + d;
    double nb = n.dot(b_vec) + d;
    if (na * nb > 0) return false;
    if (std::abs(na - nb) < 1e-9) return false;
    double t_val = na / (na - nb);
    if (t_val < 0.0 || t_val > 1.0) return false;
    out_pt = Point3(a.x + t_val * (b.x - a.x), a.y + t_val * (b.y - a.y), a.z + t_val * (b.z - a.z));
    return true;
}

Segment3 intersect_non_coplanar_triangles(const Triangle3& t1, const Triangle3& t2) {
    if (are_coplanar(t1, t2)) return Segment3();

    std::vector<Point3> pts;

    Point3 edges1[3][2] = {{t1.a, t1.b}, {t1.b, t1.c}, {t1.c, t1.a}};
    for (int i = 0; i < 3; ++i) {
        Point3 pt;
        if (intersect_segment_plane(edges1[i][0], edges1[i][1], t2, pt)) {
            if (point_in_triangle(pt, t2)) {
                pts.push_back(pt);
            }
        }
    }

    Point3 edges2[3][2] = {{t2.a, t2.b}, {t2.b, t2.c}, {t2.c, t2.a}};
    for (int i = 0; i < 3; ++i) {
        Point3 pt;
        if (intersect_segment_plane(edges2[i][0], edges2[i][1], t1, pt)) {
            if (point_in_triangle(pt, t1)) {
                pts.push_back(pt);
            }
        }
    }

    std::vector<Point3> unique_pts;
    for (const auto& p : pts) {
        bool duplicate = false;
        for (const auto& up : unique_pts) {
            if ((p - up).length() < 1e-4) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            unique_pts.push_back(p);
        }
    }

    if (unique_pts.size() >= 2) {
        double max_dist = 0.0;
        Point3 best_a = unique_pts[0], best_b = unique_pts[1];
        for (size_t i = 0; i < unique_pts.size(); ++i) {
            for (size_t j = i + 1; j < unique_pts.size(); ++j) {
                double dist = (unique_pts[i] - unique_pts[j]).length();
                if (dist > max_dist) {
                    max_dist = dist;
                    best_a = unique_pts[i];
                    best_b = unique_pts[j];
                }
            }
        }
        if (max_dist > 1e-4) {
            return Segment3(best_a, best_b);
        }
    }

    return Segment3();
}
