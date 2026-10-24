/**
 * @file point.hpp
 * @brief 二次元幾何の点
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/geometry/point.hpp
 */
#ifndef KWM_T_GEOMETRY_POINT_HPP
#define KWM_T_GEOMETRY_POINT_HPP

#include <cstdint>
#include <cmath>

/**
 * @brief 2D integer point / vector
 *
 * 整数座標の2次元点（ベクトル）構造体。
 * 幾何・格子問題で使用。
 *
 * 典型用途:
 *   ・cross / dot
 *   ・偏角ソート
 *   ・ベクトル演算
 *
 * 使用例:
 *
 *   Point a(1,2), b(3,4);
 *   auto c = a + b;
 *   long long cr = a.cross(b);
 *
 * verified:
 *   ・https://atcoder.jp/contests/awc0079/submissions/76187291
 */

namespace kwm_t::geometry {

	template<class T>
	struct Point {

		T x;
		T y;


		constexpr Point() : x(0), y(0) {}
		constexpr Point(T x, T y) : x(x), y(y) {}

		/* lexicographic order */

		constexpr bool operator<(const Point& o) const {
			if (x != o.x) return x < o.x;
			return y < o.y;
		}

		constexpr bool operator==(const Point& o) const {
			return x == o.x && y == o.y;
		}

		constexpr bool operator!=(const Point& o) const {
			return !(*this == o);
		}

		/* vector operators */

		constexpr Point operator+(const Point& o) const {
			return { x + o.x, y + o.y };
		}

		constexpr Point operator-(const Point& o) const {
			return { x - o.x, y - o.y };
		}

		constexpr Point operator*(T k) const {
			return { x * k, y * k };
		}

		constexpr Point operator/(T k) const {
			return { x / k, y / k };
		}

		/* inner / outer products */

		auto dot(const Point& o) const {
			return x * o.x + y * o.y;
		}

		auto cross(const Point& o) const {
			return x * o.y - y * o.x;
		}

		auto dist(const Point& o) const {
			return (x - o.x) * (x - o.x) + (y - o.y) * (y - o.y);
		}

		/* squared norm */

		auto norm2() const {
			return x * x + y * y;
		}

		/* rotate 90° counterclockwise */

		constexpr Point rot90() const {
			return { -y, x };
		}

		/* rotate 90° clockwise */

		constexpr Point rot90_cw() const {
			return { y, -x };
		}
	};

	/**
	 * 任意角回転（浮動小数用）
	 */
	template<class T>
	Point<double> rotate(const Point<T>& p, double rad) {

		double c = std::cos(rad);
		double s = std::sin(rad);

		return {
			p.x * c - p.y * s,
			p.x * s + p.y * c
		};
	}

	/**
	 * 偏角ソート comparator
	 */
	template<class T>
	bool polar_cmp(const Point<T>& a, const Point<T>& b) {

		bool ha = (a.y < 0 || (a.y == 0 && a.x < 0));
		bool hb = (b.y < 0 || (b.y == 0 && b.x < 0));

		if (ha != hb) return ha > hb;

		auto cr = a.cross(b);
		if (cr != 0) return cr > 0;

		return a.norm2() < b.norm2();
	}

} // namespace kwm_t::geometry

#endif //KWM_T_GEOMETRY_POINT_HPP