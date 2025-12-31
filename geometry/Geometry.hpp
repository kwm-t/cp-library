#include<algorithm>
#include<complex>
#include<vector>
namespace geometry {
	//https://siro53.github.io/compro_library/geometry/geometry.hpp
	// 定義's
	using D = long double;
	using Point = std::complex<D>;
	const D EPS = 1e-7;
	const D PI = acosl(-1);
	Point operator*(const Point &p, const D &d) { return Point(real(p) * d, imag(p) * d); }
	D distancePoint(const Point &p, const Point &q) { return sqrt((p.imag() - q.imag()) * (p.imag() - q.imag()) + (p.real() - q.real()) * (p.real() - q.real())); }
	// double 一致判定
	bool equal(const D& lh, const D& rh) { return fabs(lh - rh) < EPS; }
	// 単位ベクトル
	Point unitVector(const Point &v) { return v / abs(v); }
	// 法線ベクトル(90度回転)
	Point normalVector(const Point &v) { return v * Point(0, 1); }
	// 内積
	D dot(const Point &lh, const Point&rh) { return lh.real() * rh.real() + lh.imag() * rh.imag(); }
	// 外積
	D cross(const Point &lh, const Point&rh) { return lh.real() * rh.imag() - lh.imag() * rh.real(); }
	// 反時計回りにθ(ラジアン)回転
	Point rotate(const Point &p, const D &theta) { return Point(cos(theta) * p.real() - sin(theta) * p.imag(), sin(theta) * p.real() + cos(theta) * p.imag()); }
	// ラジアン->度
	D rTOd(const D &radian) { return radian * 180.0 / PI; }
	// 度->ラジアン 
	D dTOr(const D & degree) { return degree * PI / 180.0; }
	// 位置関係(aを基準)
	int positional(const Point &a, Point b, Point c) {
		b -= a, c -= a;
		//a,b,cが反時計回りに存在する
		if (cross(b, c) > EPS)return 1;
		//a,b,cが時計回りに存在する
		if (cross(b, c) < -EPS)return -1;
		// c-a-b
		if (dot(b, c) < 0)return 2;
		// a-b-c
		if (norm(b) < norm(c))return -2;
		// a-c-b
		return 0;
	}
	// 構造体's
	// 直線(a,bの2点を通る)
	struct Line {
		Point a, b;
		Line() = default;
		Line(Point _a, Point _b) :a(_a), b(_b) {}
		// ax + by = c
		Line(D _a, D _b, D _c) {
			if (equal(_a, 0)) a = Point(0, _c / _b), b = Point(1, _c / _b);
			else if (equal(_b, 0)) a = Point(_c / _a, 0), b = Point(_c / _a, 1);
			else if (equal(_c, 0)) a = Point(), b = Point(_b, -_a);
			else a = Point(0, _c / _b), b = Point(_c / _a, 0);
		}
	};
	// 線分
	struct Segment :Line {
		Segment() = default;
		Segment(Point a, Point b) :Line(a, b) {}
		D SegSize() { return abs(a - b); }
	};
	// 円
	struct Circle {
		Point p;
		D r;
		Circle() = default;
		Circle(Point _p, D _r) :p(_p), r(_r) {}
	};
	// ２直線の直行判定(内積 = 0)
	bool isOrthogonal(const Line & lh, const Line & rh) { return equal(0, dot(lh.b - lh.a, rh.b - rh.a)); }
	// ２直線の並行判定(外積 = 0)
	bool isParallel(const Line & lh, const Line & rh) { return equal(0, cross(lh.b - lh.a, rh.b - rh.a)); }
	// 点cが直線ab上にあるか
	bool isPointOnLine(const Point &a, const Point &b, const Point &c) { return isParallel(Line(a, b), Line(a, c)); }
	// 点cが線分ab上にあるか
	bool isPointOnSegment(const Point &a, const Point &b, const Point &c) { return (abs(a - c) + abs(c - b) < abs(a - b) + EPS); }
	// 直線と点の距離
	D distanceLineAndPoint(const Line &l, const Point &p) { return abs(cross(l.b - l.a, p - l.a)) / abs(l.b - l.a); }
	// 線分と点の距離
	D distanceSegmentAndPoint(const Segment &l, const Point &p) {
		if (dot(l.b - l.a, p - l.a) < EPS) return abs(p - l.a);
		if (dot(l.a - l.b, p - l.b) < EPS) return abs(p - l.b);
		return distanceLineAndPoint(l, p);
	}
	// 直線と直線の交点
	Point crossPoint(const Line &s, const Line &t) {
		D d1 = cross(s.b - s.a, t.b - t.a);
		D d2 = cross(s.b - s.a, s.b - t.a);
		if (equal(abs(d1), 0) && equal(abs(d2), 0)) return t.a;
		return t.a + (t.b - t.a) * (d2 / d1);
	}
	// 線分と線分の交点
	Point crossPoint(const Segment &s, const Segment &t) {
		return crossPoint(Line(s), Line(t));
	}
	// 線分sと線分tが交差しているかどうか
	// bound:線分の端点を含むか
	bool isIntersect(const Segment &s, const Segment &t, bool bound) {
		return positional(s.a, s.b, t.a) * positional(s.a, s.b, t.b) < bound &&
			positional(t.a, t.b, s.a) * positional(t.a, t.b, s.b) < bound;
	}
	// 線分sとtの距離
	D distanceBetweenSegments(const Segment &s, const Segment &t) {
		if (isIntersect(s, t, 1)) return (D)(0);
		D ans = distanceSegmentAndPoint(s, t.a);
		chmin(ans, distanceSegmentAndPoint(s, t.b));
		chmin(ans, distanceSegmentAndPoint(t, s.a));
		chmin(ans, distanceSegmentAndPoint(t, s.b));
		return ans;
	}
	// 射影(projection)
	// 直線(線分)lに点pから引いた垂線の足を求める
	Point projection(const Line &l, const Point &p) {
		D t = dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
		return l.a + (l.a - l.b) * t;
	}

	Point projection(const Segment &l, const Point &p) {
		D t = dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
		return l.a + (l.a - l.b) * t;
	}

	// 反射(reflection)
	// 直線lを対称軸として点pと線対称の位置にある点を求める
	Point reflection(const Line &l, const Point &p) {
		return p + (projection(l, p) - p) * (D)2.0;
	}

	// 2つの円の交差判定
	// 返り値は共通接線の数
	int isIntersect(const Circle &c1, const Circle &c2) {
		D d = abs(c1.p - c2.p);
		// 2つの円が離れている場合
		if (d > c1.r + c2.r + EPS) return 4;
		// 外接している場合
		if (equal(d, c1.r + c2.r)) return 3;
		// 内接している場合
		if (equal(d, abs(c1.r - c2.r))) return 1;
		// 内包している場合
		if (d < abs(c1.r - c2.r) - EPS) return 0;
		return 2;
	}

	// 2つの円の交点
	std::vector<Point> crossPoint(const Circle &c1, const Circle &c2) {
		std::vector<Point> res;
		int mode = isIntersect(c1, c2);
		// 2つの中心の距離
		D d = abs(c1.p - c2.p);
		// 2円が離れている場合
		if (mode == 4) return res;
		// 1つの円がもう1つの円に内包されている場合
		if (mode == 0) return res;
		// 2円が外接する場合
		if (mode == 3) {
			D t = c1.r / (c1.r + c2.r);
			res.emplace_back(c1.p + (c2.p - c1.p) * t);
			return res;
		}
		// 内接している場合
		if (mode == 1) {
			if (c2.r < c1.r - EPS) {
				res.emplace_back(c1.p + (c2.p - c1.p) * (c1.r / d));
			}
			else {
				res.emplace_back(c2.p + (c1.p - c2.p) * (c2.r / d));
			}
			return res;
		}
		// 2円が重なる場合
		D rc1 = (c1.r * c1.r + d * d - c2.r * c2.r) / (2 * d);
		D rs1 = sqrt(c1.r * c1.r - rc1 * rc1);
		if (c1.r - abs(rc1) < EPS) rs1 = 0;
		Point e12 = (c2.p - c1.p) / abs(c2.p - c1.p);
		res.emplace_back(c1.p + rc1 * e12 + rs1 * e12 * Point(0, 1));
		res.emplace_back(c1.p + rc1 * e12 + rs1 * e12 * Point(0, -1));
		return res;
	}

	// 点pが円cの内部(円周上も含む)に入っているかどうか
	bool isInCircle(const Circle &c, const Point &p) {
		D d = abs(c.p - p);
		return (equal(d, c.r) || d < c.r - EPS);
	}

	// 円cと直線lの交点
	std::vector<Point> crossPoint(const Circle &c, const Line &l) {
		std::vector<Point> res;
		D d = distanceLineAndPoint(l, c.p);
		// 交点を持たない
		if (d > c.r + EPS) return res;
		// 接する
		Point h = projection(l, c.p);
		if (equal(d, c.r)) {
			res.emplace_back(h);
			return res;
		}
		Point e = unitVector(l.b - l.a);
		D ph = sqrt(c.r * c.r - d * d);
		res.emplace_back(h - e * ph);
		res.emplace_back(h + e * ph);
		return res;
	}
	// 絶対２つ返す
	std::pair<Point, Point> crosspoint(const Circle &c, const Line l) {
		Point pr = projection(l, c.p);
		Point e = (l.b - l.a) / abs(l.b - l.a);
		if (equal(distanceLineAndPoint(l, c.p), c.r)) return { pr, pr };
		double base = sqrt(c.r * c.r - norm(pr - c.p));
		return { pr - e * base, pr + e * base };
	}

	// 点pを通る円cの接線
	// 2本あるので、接点のみを返す
	std::vector<Point> tangentToCircle(const Point &p, const Circle &c) {
		return crossPoint(c, Circle(p, sqrt(norm(c.p - p) - c.r * c.r)));
	}

	// 円の共通接線
	std::vector<Line> tangent(const Circle &a, const Circle &b) {
		std::vector<Line> ret;
		// 2円の中心間の距離
		D g = abs(a.p - b.p);
		// 円が内包されている場合
		if (equal(g, 0)) return ret;
		Point u = unitVector(b.p - a.p);
		Point v = rotate(u, PI / 2);
		for (int s : {-1, 1}) {
			D h = (a.r + b.r * s) / g;
			if (equal(h * h, 1)) {
				ret.emplace_back(a.p + (h > 0 ? u : -u) * a.r,
					a.p + (h > 0 ? u : -u) * a.r + v);

			}
			else if (1 - h * h > 0) {
				Point U = u * h, V = v * sqrt(1 - h * h);
				ret.emplace_back(a.p + (U + V) * a.r,
					b.p - (U + V) * (b.r * s));
				ret.emplace_back(a.p + (U - V) * a.r,
					b.p - (U - V) * (b.r * s));
			}
		}
		return ret;
	}

	// 多角形の面積を求める
	D PolygonArea(const std::vector<Point> &p) {
		D res = 0;
		int n = p.size();
		for (int i = 0; i < n; i++) res += cross(p[i], p[(i + 1) % n]);
		return res * 0.5;
	}

	// 凸多角形かどうか
	bool isConvex(const std::vector<Point> &p) {
		int n = p.size();
		int now, pre, nxt;
		for (int i = 0; i < n; i++) {
			pre = (i - 1 + n) % n;
			nxt = (i + 1) % n;
			now = i;
			if (positional(p[pre], p[now], p[nxt]) == -1) return false;
		}
		return true;
	}

	// 凸包 O(NlogN)
	std::vector<Point> ConvexHull(std::vector<Point> p) {
		int n = (int)p.size(), k = 0;
		std::sort(p.begin(), p.end(), [](const Point &a, const Point &b) {
			return (real(a) != real(b) ? real(a) < real(b) : imag(a) < imag(b));
			});
		std::vector<Point> ch(2 * n);
		// 一直線上の3点を含める -> (< -EPS)
		// 含め無い -> (< EPS)
		for (int i = 0; i < n; ch[k++] = p[i++]) { // lower
			while (k >= 2 &&
				cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < EPS)
				--k;
		}
		for (int i = n - 2, t = k + 1; i >= 0; ch[k++] = p[i--]) { // upper
			while (k >= t &&
				cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < EPS)
				--k;
		}
		ch.resize(std::max(0, k - 1));
		return ch;
	}

	// 多角形gに点pが含まれているか?
	// 含まれる:2, 辺上にある:1, 含まれない:0
	int isContained(const std::vector<Point> &g, const Point &p) {
		bool in = false;
		int n = (int)g.size();
		for (int i = 0; i < n; i++) {
			Point a = g[i] - p, b = g[(i + 1) % n] - p;
			if (imag(a) > imag(b)) swap(a, b);
			if (imag(a) <= EPS && EPS < imag(b) && cross(a, b) < -EPS) in = !in;
			if (cross(a, b) == 0 && dot(a, b) <= 0) return 1;
		}
		return (in ? 2 : 0);
	}

	// 凸多角形pを直線lで切断し、その左側を返す
	std::vector<Point> ConvexCut(std::vector<Point> p, Line l) {
		std::vector<Point> ret;
		int sz = (int)p.size();
		for (int i = 0; i < sz; ++i) {
			Point now = p[i];
			Point nxt = p[i == sz - 1 ? 0 : i + 1];
			if (positional(l.a, l.b, now) != -1) ret.emplace_back(now);
			if (positional(l.a, l.b, now) * positional(l.a, l.b, nxt) < 0) {
				ret.emplace_back(crossPoint(Line(now, nxt), l));
			}
		}
		return ret;
	}
}
using namespace geometry;