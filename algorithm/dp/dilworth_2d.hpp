#ifndef KWM_T_ALGORITHM_DP_DILWORTH_2D_HPP
#define KWM_T_ALGORITHM_DP_DILWORTH_2D_HPP

#include <vector>
#include <algorithm>
#include <utility>

//#include "algorithm/dp/bitonic_subsequence.hpp"

namespace kwm_t::algorithm::dp {

/**
 * @brief Dilworth (2D) - 最大反鎖サイズ
 *
 * @details
 *  2次元点列 v = {(x_i, y_i)} に対して、
 *  poset:
 *      (x1, y1) <= (x2, y2) ⇔ x1 <= x2 && y1 <= y2
 *
 *  における最大反鎖サイズを求める。
 *
 *  実装:
 *    - x 昇順, x が同じなら y 降順にソート
 *    - y に対する LIS を取る
 *
 *  注意:
 *    tie-break（y 降順）が重要（重複カウント防止）
 *
 * Complexity:
 *  O(N log N)
 *
 * @tparam T, U
 * @param v 点列（コピーされる）
 * @param strict
 *   true  : strictly increasing (y1 < y2)
 *   false : non-decreasing   (y1 <= y2)
 *
 * @return 最大反鎖サイズ
 * 
 * verified
 *  https://atcoder.jp/contests/abc457/submissions/75676955
 */
template<class T, class U>
int dilworth_2d(std::vector<std::pair<T, U>> v, bool strict = true) {

	std::sort(v.begin(), v.end());

	std::vector<U> ys;
	ys.reserve(v.size());

	for (const auto& p : v) ys.push_back(-p.second);

	return lis(ys, strict);
}


/**
 * @brief Dilworth (2D) - 反鎖復元（インデックス）
 *
 * @details
 *  最大反鎖を構成する点のインデックスを返す。
 *  （元配列 v に対するインデックス）
 *
 * Complexity:
 *  O(N log N)
 *
 * @return インデックス列
 */
template<class T, class U>
std::vector<int> dilworth_2d_restore(const std::vector<std::pair<T, U>>& v, bool strict = true) {

	int n = (int)v.size();

	std::vector<int> ord(n);
	for (int i = 0; i < n; ++i) ord[i] = i;

	std::sort(ord.begin(), ord.end(), [&](int i, int j) {
		return v[i] < v[j];
		});

	std::vector<U> ys(n);
	for (int i = 0; i < n; ++i) ys[i] = -v[ord[i]].second;

	auto idx = lis_restore(ys, strict);

	std::vector<int> res;
	res.reserve(idx.size());

	for (int i : idx) res.push_back(ord[i]);

	return res;
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_DILWORTH_2D_HPP