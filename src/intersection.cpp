#include "intersection.hpp"
#include <cmath>
#include <algorithm>

const double EPS = 1e-6;

bool are_coplanar(const Triangle3& t1, const Triangle3& t2) {
    Vector3 n1 = t1.normal();
    Vector3 n2 = t2.normal();
    // Проверка коллинеарности нормалей
    if (n1.cross(n2).length() > EPS) return false;
    // Проверка расстояния от вершины второго до плоскости первого
    double dist = std::abs(n1.dot(t2.a - t1.a));
    return dist < EPS;
}

struct Point2D { double x, y; };

// Ориентированная площадь для определения стороны
double cross2d(Point2D a, Point2D b, Point2D c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Пересечение прямых в 2D (Метод Крамера)
Point2D intersect_lines2d(Point2D a, Point2D b, Point2D p, Point2D q) {
    double A1 = b.y - a.y, B1 = a.x - b.x, C1 = A1 * a.x + B1 * a.y;
    double A2 = q.y - p.y, B2 = p.x - q.x, C2 = A2 * p.x + B2 * p.y;
    double det = A1 * B2 - A2 * B1;
    if (std::abs(det) < EPS) return a; // Параллельны (fallback)
    return {(B2 * C1 - B1 * C2) / det, (A1 * C2 - A2 * C1) / det};
}

Polygon3 intersect_coplanar_triangles(const Triangle3& t1, const Triangle3& t2) {
    if (t1.is_degenerate() || t2.is_degenerate()) return Polygon3();
    if (!are_coplanar(t1, t2)) return Polygon3();

    Vector3 n = t1.normal();
    // Выбор наибольшей компоненты нормали для проекции на 2D-плоскость
    int drop_axis = 0; // 0=X, 1=Y, 2=Z
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

    // Гарантируем, что отсекающий полигон (poly2) задан против часовой стрелки
    if (cross2d(poly2[0], poly2[1], poly2[2]) < 0) {
        std::reverse(poly2.begin(), poly2.end());
    }

    // Алгоритм Сазерленда-Ходжмана для отсечения
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
