/**
 * @file longest_increasing_subsequence.hpp
 * @brief 最長増加部分列
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/dp/longest_increasing_subsequence.hpp
 */

#ifndef KWM_T_ALGORITHM_DP_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define KWM_T_ALGORITHM_DP_LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <vector>
#include <algorithm>
#include <limits>

/**
 * @brief Longest Increasing Subsequence (LIS)
 *
 * @details
 *  配列の最長増加部分列を O(N log N) で求める。
 *
 * Typical Use:
 *  - LIS の長さ
 *  - LIS の復元
 *
 * Complexity:
 *  Time: O(N log N)
 *  Memory: O(N)
 *
 * Template Parameters:
 *  T : 要素の型
 *
 * Parameters:
 *  a : 入力配列
 *  is_strong :
 *      true  → strictly increasing
 *      false → non-decreasing
 *
 * Return:
 *  lis(): LIS の長さ
 *  lis_restore(): LIS のインデックス列
 *
 * Notes:
 *  dp[i] := 長さ i+1 の LIS の末尾の最小値
 *
 * Example:
 *  ```cpp
 *  vector<int> a = {3,1,4,1,5,9};
 *
 *  int len = algorithm::dp::lis(a);
 *
 *  auto idx = algorithm::dp::lis_restore(a);
 *  ```
 *
 * Verified:
 *
 */

namespace kwm_t::algorithm::dp {

/**
 * @brief LIS の長さ
 */
template<class T>
int lis(const std::vector<T>& a, bool is_strong = true) {

	int n = (int)a.size();
	const T INF = std::numeric_limits<T>::max();

	std::vector<T> dp(n, INF);

	for (const auto& x : a) {
		if (is_strong) *std::lower_bound(dp.begin(), dp.end(), x) = x;
		else *std::upper_bound(dp.begin(), dp.end(), x) = x;
	}

	return std::lower_bound(dp.begin(), dp.end(), INF) - dp.begin();
}


/**
 * @brief LIS を復元（インデックス）
 */
template<class T>
std::vector<int> lis_restore(const std::vector<T>& a, bool is_strong = true) {

	int n = (int)a.size();

	const T INF = std::numeric_limits<T>::max();

	std::vector<T> dp(n, INF);
	std::vector<int> pos(n);
	std::vector<int> pre(n, -1);

	int len = 0;

	for (int i = 0; i < n; ++i) {

		int k;

		if (is_strong) k = std::lower_bound(dp.begin(), dp.begin() + len, a[i]) - dp.begin();
		else k = std::upper_bound(dp.begin(), dp.begin() + len, a[i]) - dp.begin();

		dp[k] = a[i];
		pos[k] = i;

		if (k > 0) pre[i] = pos[k - 1];

		if (k == len) ++len;
	}

	std::vector<int> res(len);

	int cur = pos[len - 1];

	for (int i = len - 1; i >= 0; --i) {
		res[i] = cur;
		cur = pre[cur];
	}

	return res;
}

}

#endif // KWM_T_ALGORITHM_DP_LONGEST_INCREASING_SUBSEQUENCE_HPP