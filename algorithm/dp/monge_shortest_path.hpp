/**
 * @file monge_shortest_path.hpp
 * @brief Monge 性を利用した最短路 DP
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/dp/monge_shortest_path.hpp
 */
#ifndef KWM_T_ALGORITHM_DP_MONGE_SHORTEST_PATH_HPP
#define KWM_T_ALGORITHM_DP_MONGE_SHORTEST_PATH_HPP

#include <vector>
#include <limits>
#include <utility>
#include "monotone_minima.hpp"

namespace kwm_t::algorithm::dp {

/**
 * @brief Monge shortest path (Divide & Conquer + Monotone Minima)
 *
 * DP:
 *   dist[j] = min_{i < j} (dist[i] + cost(i, j))
 *
 * を Monge 性を利用して高速化する。
 *
 * ----------------------------------------
 * ■ 概要
 *
 * 通常このDPは O(N^2) だが、
 *
 *   ・cost が Monge（四角不等式）
 *   ・argmin が単調（Knuth的条件）
 *
 * を満たすとき、
 *
 *   monotone minima + 分割統治
 *
 * により O(N log N) 程度で計算できる。
 *
 * ----------------------------------------
 * ■ Monge性（重要）
 *
 * cost が以下を満たす：
 *
 *   cost(a, c) + cost(b, d) <= cost(a, d) + cost(b, c)
 *   (a <= b <= c <= d)
 *
 * これにより argmin が単調になる：
 *
 *   opt[j] <= opt[j+1]
 *
 * ----------------------------------------
 * ■ 典型用途
 *
 * ・区間DP最適化
 * ・k分割問題（+ λ最適化と併用）
 * ・L1距離系コスト
 * ・中央値系コスト
 *
 * ----------------------------------------
 * ■ 引数
 *
 * @tparam T    距離型（long long など）
 * @tparam Cost cost(i, j) を返す関数
 *
 * @param N     頂点数（0-indexedで 0 ... N-1）
 * @param cost  遷移コスト関数
 *
 * ----------------------------------------
 * ■ 戻り値
 *
 * dist 配列：
 *   dist[j] = min_{i < j} (dist[i] + cost(i, j))
 *
 * 初期条件：
 *   dist[0] = 0
 *
 * ----------------------------------------
 * ■ 計算量
 *
 * O(N log N)
 *
 * （monotone minima が O(N) × log 分割）
 *
 * ----------------------------------------
 * ■ 制約 / 注意（かなり重要）
 *
 * ・cost は Monge 性を満たす必要あり
 * ・argmin が単調でないと壊れる
 * ・cost(i, j) は i < j のみ意味を持つ前提
 * ・cost は O(1) で計算できる必要あり
 *
 * ----------------------------------------
 * ■ 使用例
 *
 * auto dist = monge_shortest_path<long long>(N, [&](int i, int j) {
 *     return cost(i, j);
 * });
 *
 * ----------------------------------------
 * ■ verified
 *
 * ・https://atcoder.jp/contests/abc355/submissions/53956239
 *
 */
template <typename T, typename Cost>
std::vector<T> monge_shortest_path(int N, const Cost& cost) {
	const T INF = std::numeric_limits<T>::max() / 2;

	std::vector<T> dist(N, INF);
	dist[0] = 0;

	// divide & conquer
	auto dfs = [&](auto& self, int l, int r) -> void {
		if (r - l == 1) return;

		int m = (l + r) >> 1;

		self(self, l, m);

		// f(i, j): 左側 index → 右側 index
		auto f = [&](int i, int j) -> T {
			return dist[l + j] + cost(l + j, m + i);
		};

		auto idx = kwm_t::algorithm::dp::monotone_minima<T>(r - m, m - l, f);

		for (int i = 0; i < r - m; ++i) {
			int j = idx[i].first;
			T val = f(i, j);
			if (val < dist[m + i]) dist[m + i] = val;
		}

		self(self, m, r);
	};

	dfs(dfs, 0, N);

	return dist;
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_MONGE_SHORTEST_PATH_HPP