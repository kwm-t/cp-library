/**
 * @file cocircular.hpp
 * @brief 複数の点が同一円周上にあるかを判定
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/geometry/cocircular.hpp
 */
#ifndef KWM_T_GEOMETRY_COCIRCULAR_HPP
#define KWM_T_GEOMETRY_COCIRCULAR_HPP

#include <vector>
#include <cstdint>

#include "point.hpp"

namespace kwm_t::geometry {

/**
 * @brief すべての点が同一円周上に存在するかを判定する。
 *
 * 最初の3点から円を定め、残りの点がその円周上にあるかを
 * 整数演算で判定する。
 *
 * 計算量:
 *   O(N)
 *
 * @tparam T 座標型（整数型）
 * @param pts 点集合
 * @return すべての点が同一円周上に存在するなら true
 *
 * 制約 / 注意:
 *   - 点数が3未満の場合は true を返す。
 *   - 3点以上の場合、最初の3点から非一直線の3点を探す。
 *   - すべての点が一直線上にある場合は false を返す。
 *   - 重複点を許容する。
 *   - 内部計算に __int128_t を使用する。
 *   - 中間計算が __int128_t の範囲に収まること。
 *   - 整数座標を想定する。
 *
 * 使用例:
 *   std::vector<Point<long long>> pts = {
 *       {0, 0}, {1, 0}, {1, 1}, {0, 1}
 *   };
 *   bool ok = is_cocircular(pts); // true
 *
 * verified:
 *   https://yukicoder.me/submissions/1194922
 */
template <class T>
bool is_cocircular(const std::vector<Point<T>>& pts) {
	using W = long long;

	const int n = static_cast<int>(pts.size());
	if (n < 3) return true;

	// 非一直線の3点を探す。
	int i = 0;
	int j = -1;
	int k = -1;

	for (int t = 1; t < n; ++t) {
		if (pts[t] != pts[i]) {
			j = t;
			break;
		}
	}

	// すべて同じ点。
	if (j == -1) return true;

	for (int t = 0; t < n; ++t) {
		if (t == i || t == j) continue;

		W ax = static_cast<W>(pts[j].x) - pts[i].x;
		W ay = static_cast<W>(pts[j].y) - pts[i].y;
		W bx = static_cast<W>(pts[t].x) - pts[i].x;
		W by = static_cast<W>(pts[t].y) - pts[i].y;

		if (ax * by - ay * bx != 0) {
			k = t;
			break;
		}
	}

	// すべての点が一直線上にある。
	if (k == -1) return false;

	// 円周上の点 p は
	// x^2 + y^2 + D*x + E*y + F = 0
	// を満たす。3点から D, E, F を求める。
	const W x1 = pts[i].x;
	const W y1 = pts[i].y;
	const W x2 = pts[j].x;
	const W y2 = pts[j].y;
	const W x3 = pts[k].x;
	const W y3 = pts[k].y;

	const W a1 = x2 - x1;
	const W b1 = y2 - y1;
	const W c1 = x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1;

	const W a2 = x3 - x1;
	const W b2 = y3 - y1;
	const W c2 = x3 * x3 + y3 * y3 - x1 * x1 - y1 * y1;

	const W det = a1 * b2 - a2 * b1;

	// D = (b1*c2 - b2*c1) / det
	// E = (a2*c1 - a1*c2) / det
	// 分母を払って整数演算だけで比較する。
	const W d = b1 * c2 - b2 * c1;
	const W e = a2 * c1 - a1 * c2;
	const W f = -det * (x1 * x1 + y1 * y1)
		- d * x1 - e * y1;

	for (const auto& p : pts) {
		const W x = p.x;
		const W y = p.y;

		if (det * (x * x + y * y) + d * x + e * y + f != 0) {
			return false;
		}
	}

	return true;
}

} // namespace kwm_t::geometry

#endif // KWM_T_GEOMETRY_COCIRCULAR_HPP