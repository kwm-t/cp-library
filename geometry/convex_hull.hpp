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
 *   AOJ CGL_4_A
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

}

#endif