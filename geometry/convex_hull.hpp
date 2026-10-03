#ifndef KWM_T_GEOMETRY_CONVEX_HULL_HPP
#define KWM_T_GEOMETRY_CONVEX_HULL_HPP

#include <vector>
#include <algorithm>
#include "point.hpp"

/**
 * @brief Convex Hull (Monotone Chain)
 *
 * 点集合の凸包を求める。
 *
 * 典型用途:
 *   ・凸包
 *   ・polygon処理
 *   ・rotating calipers
 *
 * 計算量:
 *   O(N log N)
 *
 * @tparam T 座標型
 *
 * @param pts 点集合
 * @param strict
 *   true  : 同一直線上の点を除く
 *   false : 同一直線上の点を含む
 *
 * @return 凸包（反時計回り）
 *
 * verified:
 *   https://atcoder.jp/contests/awc0103/submissions/77111025
 */

namespace kwm_t::geometry {

template<class T>
std::vector<Point<T>> convex_hull(std::vector<Point<T>> pts, bool strict = true) {

	int n = pts.size();
	if (n <= 1) return pts;

	std::sort(pts.begin(), pts.end());

	std::vector<Point<T>> hull;
	hull.reserve(n * 2);

	auto check = [&](auto v) {
		return strict ? (v <= 0) : (v < 0);
	};

	// lower hull
	for (const auto& p : pts) {
		while (hull.size() >= 2 &&
			check((hull.back() - hull[hull.size() - 2])
				.cross(p - hull.back()))) {
			hull.pop_back();
		}
		hull.push_back(p);
	}

	int lower_size = hull.size();

	// upper hull
	for (int i = n - 2; i >= 0; --i) {

		const auto& p = pts[i];

		while ((int)hull.size() > lower_size &&
			check((hull.back() - hull[hull.size() - 2])
				.cross(p - hull.back()))) {
			hull.pop_back();
		}

		hull.push_back(p);
	}

	hull.pop_back();

	return hull;
}

/**
 * @brief 凸包の頂点列を、辺ベクトルが polar_cmp 順になるように巡回シフトする。
 *
 * convex_hull() が返す凸包は反時計回りの巡回列であるため、
 * 辺ベクトルも角度順には並んでいるが、始点によっては
 * polar_cmp の順序をまたいでしまう。
 *
 * 例えば正方形の辺ベクトルが
 *
 *   (1, 0), (0, 1), (-1, 0), (0, -1)
 *
 * の順に並んでいる場合、polar_cmp では
 *
 *   (0, -1), (1, 0), (0, 1), (-1, 0)
 *
 * の順になるため、そのままでは std::merge に使えない。
 *
 * 本関数は、辺ベクトルのうち polar_cmp で最小のものを
 * 先頭の辺にすることで、凸包の辺ベクトル列全体を
 * polar_cmp に関して昇順にする。
 *
 * 主な用途は Minkowski Sum の計算で、複数の凸多角形の
 * 辺ベクトル列を std::merge するときに使用する。
 *
 * @tparam T 点の座標型
 * @param hull 反時計回りに並んだ凸包の頂点列
 */
template <class T>
void rotate_min_edge(std::vector<Point<T>>& hull) {
	const int n = static_cast<int>(hull.size());
	if (n <= 1) return;

	int pos = 0;
	for (int i = 1; i < n; ++i) {
		const Point<T> edge = hull[(i + 1) % n] - hull[i];
		const Point<T> min_edge = hull[(pos + 1) % n] - hull[pos];

		if (polar_cmp(edge, min_edge)) {
			pos = i;
		}
	}

	std::rotate(hull.begin(), hull.begin() + pos, hull.end());
}

}
#endif