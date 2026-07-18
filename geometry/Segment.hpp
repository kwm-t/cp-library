#ifndef KWM_T_GEOMETRY_SEGMENT_HPP
#define KWM_T_GEOMETRY_SEGMENT_HPP

#include <tuple>
#include <vector>
#include <numeric>
#include <type_traits>
#include <cstdlib>

#include "point.hpp"

/**
 * @brief Line utilities (integer normalized form)
 *
 * 直線を
 *
 *      ax + by + c = 0
 *
 * の整数正規形で扱う。
 *
 * 正規化条件:
 *   gcd(|a|,|b|,|c|) = 1
 *   a > 0  または
 *   a = 0 かつ b > 0
 *   a = b = 0 の場合は c >= 0
 *
 * 同一直線なら必ず同じ (a,b,c) を返す。
 */

namespace kwm_t::geometry {

/**
 * @brief 直線を正規化する
 *
 * ax + by + c = 0 を正規化して返す。
 *
 * @tparam T 整数型
 *
 * @param a,b,c 直線係数
 *
 * @return 正規化後の (a,b,c)
 */
template<class T>
inline std::tuple<T, T, T>
normalize_line(T a, T b, T c) {

	static_assert(std::is_integral_v<T>, "T must be integral");

	T g = std::gcd(std::abs(a),
		std::gcd(std::abs(b), std::abs(c)));

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

	return { a, b, c };
}

/**
 * @brief 2点を通る直線（正規化）
 */
template<class T>
inline std::tuple<T, T, T>
line_from_points(const Point<T>& p1, const Point<T>& p2) {

	static_assert(std::is_integral_v<T>, "T must be integral");

	T a = p2.y - p1.y;
	T b = p1.x - p2.x;
	T c = p1.y * p2.x - p1.x * p2.y;

	return normalize_line(a, b, c);
}

/**
 * @brief 2点を結ぶ線分の垂直二等分線（正規化）
 *
 * p1 != p2 を仮定。
 * verify
 *  https://atcoder.jp/contests/abc467/submissions/77650914
 */
template<class T>
inline std::tuple<T, T, T>
perpendicular_bisector(const Point<T>& p1, const Point<T>& p2) {

	static_assert(std::is_integral_v<T>, "T must be integral");

	T dx = p2.x - p1.x;
	T dy = p2.y - p1.y;

	return normalize_line(
		dx * 2,
		dy * 2,
		-(dx * (p1.x + p2.x) + dy * (p1.y + p2.y))
	);
}

/**
 * @brief 2直線が交点を持つか判定
 *
 * @tparam IncludeCoincident
 *   true  : 同一直線も true
 *   false : 一意な交点を持つ場合のみ true
 *
 * 正規化済みである必要はない。
 *
 * verify
 *  https://atcoder.jp/contests/abc467/submissions/77650914
 */
template<bool IncludeCoincident = true, class T>
inline bool
has_intersection(const std::tuple<T, T, T>& l1,
	const std::tuple<T, T, T>& l2) {

	static_assert(std::is_integral_v<T>, "T must be integral");

	const auto& [a1, b1, c1] = l1;
	const auto& [a2, b2, c2] = l2;

	const T det = a1 * b2 - a2 * b1;

	if (det != 0) return true;

	if constexpr (!IncludeCoincident) {
		return false;
	}

	return
		a1 * c2 == a2 * c1 &&
		b1 * c2 == b2 * c1;
}

/**
 * @brief 全点が同一直線上にあるか判定
 *
 * 点数が 0,1,2 の場合は true を返す。
 *
 * 計算量:
 *   O(N)
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

		if (dx * dy2 - dy * dx2 != 0) {
			return false;
		}
	}

	return true;
}

} // namespace kwm_t::geometry

#endif // KWM_T_GEOMETRY_SEGMENT_HPP