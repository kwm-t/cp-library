#ifndef KWM_T_GEOMETRY_LINE_FROM_POINTS_HPP
#define KWM_T_GEOMETRY_LINE_FROM_POINTS_HPP

#include <tuple>
#include <numeric>
#include <type_traits>
#include <cstdlib>

//#include "point.hpp"

/**
 * @brief Line from two points (normalized)
 *
 * 2点 p1, p2 を通る直線を
 *
 *      ax + by + c = 0
 *
 * の整数正規形で返す。
 *
 * 正規化条件:
 *   gcd(|a|,|b|,|c|) = 1
 *   a > 0  または
 *   a = 0 かつ b > 0
 *   a = b = 0 の場合は c ≥ 0
 *
 * 同一直線なら必ず同じ (a,b,c) を返す。
 *
 * 典型用途:
 *   ・直線の同一判定
 *   ・直線集合のハッシュ化
 *   ・格子幾何
 *
 * 計算量:
 *   O(log C) (gcd)
 *
 * @tparam T 整数型 (long long / int / __int128 など)
 *
 * @param p1 点1
 * @param p2 点2
 *
 * @return (a,b,c) : ax + by + c = 0
 *
 * 制約 / 注意:
 *   ・T は整数型を想定
 *   ・p1 != p2 を仮定
 *
 * 使用例:
 *
 *   geometry::Point<long long> p1{0,0};
 *   geometry::Point<long long> p2{2,2};
 *
 *   auto [a,b,c] = geometry::line_from_points(p1,p2);
 *
 * verified:
 *   ・直線同一判定
 */

namespace kwm_t::geometry {
template<class T>
inline std::tuple<T, T, T>
line_from_points(const Point<T>& p1, const Point<T>& p2) {

	static_assert(std::is_integral_v<T>, "T must be integral");

	T a = p2.y - p1.y;
	T b = p1.x - p2.x;
	T c = p1.y * p2.x - p1.x * p2.y;

	T g = std::gcd(std::abs(a), std::gcd(std::abs(b), std::abs(c)));

	if (g != 0) {
		a /= g;
		b /= g;
		c /= g;
	}

	if (a < 0 || (a == 0 && b < 0) || (a == 0 && b == 0 && c < 0)) {
		a = -a;
		b = -b;
		c = -c;
	}

	return { a,b,c };
}

/**
 * @brief 全点が同一直線上にあるか判定
 *
 * points の全点が同一直線上に存在するかを判定する。
 *
 * 点数が 0,1,2 の場合は true を返す。
 *
 * 計算量:
 *   O(N)
 *
 * @tparam T 整数型
 *
 * @param points 点列
 *
 * @return true  : 全点が同一直線上
 * @return false : そうでない
 *
 * 使用例:
 *
 *   vector<Point<long long>> ps = {
 *       {0,0},
 *       {1,1},
 *       {2,2},
 *       {5,5}
 *   };
 *
 *   bool ok = geometry::is_collinear(ps);
 *
 * verified:
 *   https://atcoder.jp/contests/awc0079/submissions/76187547
 */

template<class T>
inline bool
is_collinear(const std::vector<Point<T>>& points) {

	static_assert(std::is_integral_v<T>, "T must be integral");

	const int n = (int)points.size();

	if (n <= 2) return true;

	int j = -1;

	for (int i = 1; i < n; ++i) {
		if (points[i].x != points[0].x ||
			points[i].y != points[0].y) {
			j = i;
			break;
		}
	}

	// 全点一致
	if (j == -1) return true;

	const auto& p0 = points[0];
	const auto& p1 = points[j];

	T dx = p1.x - p0.x;
	T dy = p1.y - p0.y;

	for (int i = 0; i < n; ++i) {

		T dx2 = points[i].x - p0.x;
		T dy2 = points[i].y - p0.y;

		// 外積
		if (dx * dy2 - dy * dx2 != 0) {
			return false;
		}
	}

	return true;
}
} // namespace kwm_t::geometry

#endif // KWM_T_GEOMETRY_LINE_FROM_POINTS_HPP