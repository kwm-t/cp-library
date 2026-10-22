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
* @brief LIS を復元（インデックス、辞書順最小）
*/
template<class T>
std::vector<int> lis_restore(const std::vector<T>& a, bool is_strong = true) {
	int n = (int)a.size();

	std::vector<T> xs = a;
	std::sort(xs.begin(), xs.end());
	xs.erase(std::unique(xs.begin(), xs.end()), xs.end());

	int m = (int)xs.size();
	std::vector<int> bit(m + 1, 0);
	std::vector<int> len(n);

	auto query = [&](int i) {
		int res = 0;
		for (; i > 0; i -= i & -i) {
			res = std::max(res, bit[i]);
		}
		return res;
		};

	auto update = [&](int i, int v) {
		for (; i <= m; i += i & -i) {
			bit[i] = std::max(bit[i], v);
		}
		};

	for (int i = n - 1; i >= 0; --i) {
		int x = (int)(std::lower_bound(xs.begin(), xs.end(), a[i]) - xs.begin()) + 1;
		int rev = m - x + 1;
		len[i] = 1 + query(is_strong ? rev - 1 : rev);
		update(rev, len[i]);
	}

	int rem = *std::max_element(len.begin(), len.end());
	std::vector<int> res;
	T last{};
	bool has_last = false;

	for (int i = 0; i < n; ++i) {
		if (len[i] < rem) continue;
		if (has_last && (is_strong ? !(last < a[i]) : !(last <= a[i]))) continue;

		res.push_back(i);
		last = a[i];
		has_last = true;
		--rem;
		if (rem == 0) break;
	}

	return res;
}
}

#endif // KWM_T_ALGORITHM_DP_LONGEST_INCREASING_SUBSEQUENCE_HPP