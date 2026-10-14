/**
 * @file chromatic_number.hpp
 * @brief グラフの彩色数
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/chromatic_number.hpp
 */
#ifndef KWM_T_GRAPH_BIT_DP_CHROMATIC_NUMBER_HPP
#define KWM_T_GRAPH_BIT_DP_CHROMATIC_NUMBER_HPP

#include <vector>

namespace kwm_t::graph {

/**
 * @brief 各部分集合が独立集合である場合の数を数えるDP
 *
 * cnt[S] = S に含まれる頂点集合の独立集合の個数
 *
 * 計算量:
 *   O(n * 2^n)
 */
template <class Mint>
std::vector<Mint> independent_set_count(
	const std::vector<std::vector<int>>& g
) {
	int n = g.size();
	std::vector<Mint> cnt(1 << n);

	for (int mask = 0; mask < (1 << n); ++mask) {
		if (mask == 0) {
			cnt[mask] = 1;
			continue;
		}

		int v = __builtin_ctz(mask);          // 最下位bit
		int sub = mask ^ (1 << v);            // vを除いた集合

		cnt[mask] = cnt[sub];

		for (int to : g[v]) {
			sub &= ~(1 << to);
		}

		cnt[mask] += cnt[sub];
	}

	return cnt;
}

/**
 * @brief 彩色数（chromatic number）を求める
 *
 * inclusion-exclusion により計算
 *
 * 計算量:
 *   O(n * 2^n)
 *
 * 制約:
 *   - n <= 20 程度
 */
template <class Mint>
int chromatic_number(const std::vector<std::vector<int>>& g) {
	int n = g.size();
	if (n == 0) return 0;

	auto ind = independent_set_count<Mint>(g);

	std::vector<Mint> f(1 << n);

	// (-1)^(n - |S|)
	for (int mask = 0; mask < (1 << n); ++mask) {
		if ((n - __builtin_popcount(mask)) & 1) {
			f[mask] = -1;
		}
		else {
			f[mask] = 1;
		}
	}

	for (int k = 1; k <= n; ++k) {
		Mint sum = 0;

		for (int mask = 0; mask < (1 << n); ++mask) {
			f[mask] *= ind[mask];
			sum += f[mask];
		}

		if (sum != Mint(0)) return k;
	}

	return n;
}

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_BIT_DP_CHROMATIC_NUMBER_HPP