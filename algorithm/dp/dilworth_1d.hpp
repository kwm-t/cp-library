/**
 * @file dilworth_1d.hpp
 * @brief 一次元 Dilworth の定理に関する処理
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/dp/dilworth_1d.hpp
 */
#ifndef KWM_T_ALGORITHM_DP_DILWORTH_1D_HPP
#define KWM_T_ALGORITHM_DP_DILWORTH_1D_HPP

#include <vector>

#include "algorithm/dp/longest_increasing_subsequence.hpp"

namespace kwm_t::algorithm::dp {

/**
 * @brief Dilworth (1D) - 最大反鎖サイズ
 *
 * @details
 *  列 a に対して
 *      i < j かつ a[i] <= a[j]
 *  を順序とする poset の最大反鎖サイズを求める。
 *
 *  これは最長減少部分列 (LDS) の長さに等しく、
 *  a を符号反転して LIS を取ることで求める。
 *
 * Complexity:
 *  Time: O(N log N)
 *  Memory: O(N)
 *
 * @tparam T 要素型（単項マイナスが定義されている必要あり）
 *
 * @param a 入力列
 * @param strict
 *   true  : 狭義減少（a[i] > a[j]）
 *   false : 非増加（a[i] >= a[j]）
 *
 * @return 最大反鎖サイズ
 *
 * Example:
 *  std::vector<int> a = {5, 3, 4, 2};
 *  int ans = dilworth_1d(a); // = 3 (5,4,2)
 *
 * verified:
 */
template<class T>
int dilworth_1d(const std::vector<T>& a, bool strict = true) {

	std::vector<T> b;
	b.reserve(a.size());

	for (const auto& x : a) {
		b.push_back(-x);
	}

	return lis(b, strict);
}


/**
 * @brief Dilworth (1D) - 反鎖復元（インデックス）
 *
 * @details
 *  最大反鎖（最長減少部分列）を構成するインデックス列を返す。
 *
 * Complexity:
 *  Time: O(N log N)
 *  Memory: O(N)
 *
 * @return インデックス列（元配列 a に対するもの）
 */
template<class T>
std::vector<int> dilworth_1d_restore(const std::vector<T>& a, bool strict = true) {

	std::vector<T> b;
	b.reserve(a.size());

	for (const auto& x : a) {
		b.push_back(-x);
	}

	return lis_restore(b, strict);
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_DILWORTH_1D_HPP