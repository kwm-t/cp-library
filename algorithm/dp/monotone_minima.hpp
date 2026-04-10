#ifndef KWM_T_ALGORITHM_DP_MONOTONE_MINIMA_HPP
#define KWM_T_ALGORITHM_DP_MONOTONE_MINIMA_HPP

#include <vector>
#include <utility>

namespace kwm_t::algorithm::dp {

/**
 * @brief Monotone Minima (Totally Monotone Matrix の行最小)
 *
 * monotone な n × m 行列 A に対して、
 * 各行 i の最小値をとる列 index と値を求める。
 *
 * ※ monotone: argmin が非減少
 *    argmin[i] <= argmin[i+1]
 *
 * @tparam T 値型
 * @tparam F 関数 (i, j) -> A[i][j]
 *
 * @param n 行数
 * @param m 列数
 * @param f 要素アクセス関数
 *
 * @return vector<pair<int, T>>
 *         (最小列 index, 最小値)
 *
 * @complexity O(n + m)
 *
 * @note
 * ・f は O(1) を想定
 * ・行ごとの argmin が単調である必要あり
 *
 * @example
 * auto res = monotone_minima(n, m, [&](int i, int j) {
 *     return cost(i, j);
 * });
 *
 * @verified
 * ・https://atcoder.jp/contests/abc355/submissions/53956239
 */
template<typename T, typename F>
std::vector<std::pair<int, T>> monotone_minima(int n, int m, const F& f) {
	std::vector<std::pair<int, T>> result(n);

	auto dfs = [&](auto&& self, int top, int bottom, int left, int right) -> void {
		if (top > bottom) return;

		int mid = (top + bottom) >> 1;

		T best = f(mid, left);
		int arg = left;

		for (int j = left + 1; j <= right; ++j) {
			T val = f(mid, j);
			if (val < best) {
				best = val;
				arg = j;
			}
		}

		result[mid] = { arg, best };

		self(self, top, mid - 1, left, arg);
		self(self, mid + 1, bottom, arg, right);
	};

	dfs(dfs, 0, n - 1, 0, m - 1);
	return result;
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_MONOTONE_MINIMA_HPP