/**
 * @file bitonic_subsequence.hpp
 * @brief 最長ビトニック部分列
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/dp/bitonic_subsequence.hpp
 */

#ifndef KWM_T_ALGORITHM_DP_BITONIC_SUBSEQUENCE_HPP
#define KWM_T_ALGORITHM_DP_BITONIC_SUBSEQUENCE_HPP

#include <vector>
#include <algorithm>

/**
 * @brief Longest Bitonic Subsequence (LBS)
 *
 * @details
 *  単調増加した後に単調減少する最長部分列を O(N log N) で求める。
 *
 * Typical Use:
 *  - 最長ビトニック部分列の長さ
 *  - 最長ビトニック部分列の復元
 *
 * Complexity:
 *  Time: O(N log N)
 *  Memory: O(N)
 *
 * Template Parameters:
 *  T : 要素の型
 *
 * Parameters:
 *  a         : 入力配列
 *  is_strong : true なら狭義単調、false なら広義単調
 *
 * Return:
 *  bitonic_subsequence(): 最長ビトニック部分列の長さ
 *  bitonic_subsequence_restore(): インデックス列
 *
 * Notes:
 *  各位置 i を山の頂点とし、
 *  左側の最長増加部分列と右側の最長減少部分列を組み合わせる。
 *
 * Verified:
 *
 */

namespace kwm_t::algorithm::dp {

namespace bitonic_subsequence_internal {

template<class T>
struct Result {
	std::vector<int> inc;
	std::vector<int> dec;
	std::vector<int> pre;
	std::vector<int> next;
};

template<class T>
Result<T> solve(const std::vector<T>& a, bool is_strong) {
	int n = (int)a.size();

	Result<T> res;
	res.inc.resize(n);
	res.dec.resize(n);
	res.pre.assign(n, -1);
	res.next.assign(n, -1);

	std::vector<T> dp;
	std::vector<int> pos;
	dp.reserve(n);
	pos.reserve(n);

	// inc[i]: i で終わる最長増加部分列
	for (int i = 0; i < n; ++i) {
		int k;
		if (is_strong) {
			k = std::lower_bound(dp.begin(), dp.end(), a[i]) - dp.begin();
		}
		else {
			k = std::upper_bound(dp.begin(), dp.end(), a[i]) - dp.begin();
		}

		res.inc[i] = k + 1;
		if (k > 0) res.pre[i] = pos[k - 1];

		if (k == (int)dp.size()) {
			dp.push_back(a[i]);
			pos.push_back(i);
		}
		else {
			dp[k] = a[i];
			pos[k] = i;
		}
	}

	// dec[i]: i から始まる最長減少部分列
	// 逆向きに走査することで、増加部分列の問題に帰着する。
	dp.clear();
	pos.clear();

	for (int i = n - 1; i >= 0; --i) {
		int k;
		if (is_strong) {
			k = std::lower_bound(dp.begin(), dp.end(), a[i]) - dp.begin();
		}
		else {
			k = std::upper_bound(dp.begin(), dp.end(), a[i]) - dp.begin();
		}

		res.dec[i] = k + 1;
		if (k > 0) res.next[i] = pos[k - 1];

		if (k == (int)dp.size()) {
			dp.push_back(a[i]);
			pos.push_back(i);
		}
		else {
			dp[k] = a[i];
			pos[k] = i;
		}
	}

	return res;
}

} // namespace bitonic_subsequence_internal


/**
 * @brief 最長ビトニック部分列の長さ
 */
template<class T>
int bitonic_subsequence(const std::vector<T>& a, bool is_strong = true) {
	int n = (int)a.size();
	if (n == 0) return 0;

	auto res = bitonic_subsequence_internal::solve(a, is_strong);

	int ans = 0;
	for (int i = 0; i < n; ++i) {
		ans = std::max(ans, res.inc[i] + res.dec[i] - 1);
	}
	return ans;
}


/**
 * @brief 最長ビトニック部分列を復元（インデックス）
 */
template<class T>
std::vector<int> bitonic_subsequence_restore(
	const std::vector<T>& a, bool is_strong = true
) {
	int n = (int)a.size();
	if (n == 0) return {};

	auto res = bitonic_subsequence_internal::solve(a, is_strong);

	int peak = 0;
	for (int i = 1; i < n; ++i) {
		if (res.inc[i] + res.dec[i] > res.inc[peak] + res.dec[peak]) {
			peak = i;
		}
	}

	std::vector<int> ans;

	// 山頂までの増加部分列を復元
	for (int cur = peak; cur != -1; cur = res.pre[cur]) {
		ans.push_back(cur);
	}
	std::reverse(ans.begin(), ans.end());

	// 山頂より後ろの減少部分列を復元
	for (int cur = res.next[peak]; cur != -1; cur = res.next[cur]) {
		ans.push_back(cur);
	}

	return ans;
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_BITONIC_SUBSEQUENCE_HPP