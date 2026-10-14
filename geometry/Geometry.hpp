/**
 * @file geometry.hpp
 * @brief 幾何ライブラリの基本定義
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/geometry/geometry.hpp
 */
#include <algorithm>
#include <complex>
#include <vector>
#include <cmath>

namespace geometry {

using D = long double;
using Point = std::complex<D>;

const D EPS = 1e-7;
const D PI = acosl(-1);

// ========================
// 基本
// ========================

Point operator*(const Point &p, const D &d) {
    return Point(std::real(p) * d, std::imag(p) * d);
}

D distancePoint(const Point &p, const Point &q) {
    return std::sqrt((p.imag() - q.imag()) * (p.imag() - q.imag())
        + (p.real() - q.real()) * (p.real() - q.real()));
}

bool equal(const D& lh, const D& rh) {
    return std::fabsl(lh - rh) < EPS;
}

Point unitVector(const Point &v) {
    return v / std::abs(v);
}

Point normalVector(const Point &v) {
    return v * Point(0, 1);
}

D dot(const Point &lh, const Point &rh) {
    return lh.real() * rh.real() + lh.imag() * rh.imag();
}

D cross(const Point &lh, const Point &rh) {
    return lh.real() * rh.imag() - lh.imag() * rh.real();
}

// ★ 修正ポイント（バグ）
Point rotate(const Point &p, const D &theta) {
    return Point(
        std::cos(theta) * p.real() - std::sin(theta) * p.imag(),
        std::sin(theta) * p.real() + std::cos(theta) * p.imag()
    );
}

D rTOd(const D &radian) { return radian * 180.0 / PI; }
D dTOr(const D &degree) { return degree * PI / 180.0; }

// ========================
// ccw
// ========================

int positional(const Point &a, Point b, Point c) {
    b -= a, c -= a;
    if (cross(b, c) > EPS) return 1;
    if (cross(b, c) < -EPS) return -1;
    if (dot(b, c) < 0) return 2;
    if (norm(b) < norm(c)) return -2;
    return 0;
}

// ========================
// 構造体
// ========================

struct Line {
    Point a, b;
    Line() = default;
    Line(Point _a, Point _b) : a(_a), b(_b) {}

    Line(D _a, D _b, D _c) {
        if (equal(_a, 0)) a = Point(0, _c / _b), b = Point(1, _c / _b);
        else if (equal(_b, 0)) a = Point(_c / _a, 0), b = Point(_c / _a, 1);
        else if (equal(_c, 0)) a = Point(), b = Point(_b, -_a);
        else a = Point(0, _c / _b), b = Point(_c / _a, 0);
    }
};

struct Segment : Line {
    Segment() = default;
    Segment(Point a, Point b) : Line(a, b) {}
    D SegSize() const { return std::abs(a - b); }
};

struct Circle {
    Point p;
    D r;
    Circle() = default;
    Circle(Point _p, D _r) : p(_p), r(_r) {}
};

// ========================
// 判定
// ========================

bool isOrthogonal(const Line &lh, const Line &rh) {
    return equal(0, dot(lh.b - lh.a, rh.b - rh.a));
}

bool isParallel(const Line &lh, const Line &rh) {
    return equal(0, cross(lh.b - lh.a, rh.b - rh.a));
}

bool isPointOnLine(const Point &a, const Point &b, const Point &c) {
    return isParallel(Line(a, b), Line(a, c));
}

bool isPointOnSegment(const Point &a, const Point &b, const Point &c) {
    return (std::abs(a - c) + std::abs(c - b) < std::abs(a - b) + EPS);
}

// ========================
// 距離
// ========================

D distanceLineAndPoint(const Line &l, const Point &p) {
    return std::abs(cross(l.b - l.a, p - l.a)) / std::abs(l.b - l.a);
}

D distanceSegmentAndPoint(const Segment &l, const Point &p) {
    if (dot(l.b - l.a, p - l.a) < EPS) return std::abs(p - l.a);
    if (dot(l.a - l.b, p - l.b) < EPS) return std::abs(p - l.b);
    return distanceLineAndPoint(l, p);
}

// ========================
// 交点
// ========================

Point crossPoint(const Line &s, const Line &t) {
    D d1 = cross(s.b - s.a, t.b - t.a);
    D d2 = cross(s.b - s.a, s.b - t.a);
    if (equal(std::abs(d1), 0) && equal(std::abs(d2), 0)) return t.a;
    return t.a + (t.b - t.a) * (d2 / d1);
}

Point crossPoint(const Segment &s, const Segment &t) {
    return crossPoint(Line(s), Line(t));
}

// ========================
// 線分距離（chmin除去）
// ========================

D distanceBetweenSegments(const Segment &s, const Segment &t) {
    if (positional(s.a, s.b, t.a) * positional(s.a, s.b, t.b) < 1 &&
        positional(t.a, t.b, s.a) * positional(t.a, t.b, s.b) < 1)
        return 0;

    D ans = distanceSegmentAndPoint(s, t.a);
    ans = std::min(ans, distanceSegmentAndPoint(s, t.b));
    ans = std::min(ans, distanceSegmentAndPoint(t, s.a));
    ans = std::min(ans, distanceSegmentAndPoint(t, s.b));
    return ans;
}

// ========================
// 射影・反射
// ========================

Point projection(const Line &l, const Point &p) {
    D t = dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
    return l.a + (l.a - l.b) * t;
}

Point reflection(const Line &l, const Point &p) {
    return p + (projection(l, p) - p) * (D)2.0;
}

} // namespace geometry