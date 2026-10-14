/**
 * @file bellmanford.hpp
 * @brief Bellman-Ford 法
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/shortest_path/bellmanford.hpp
 */
#ifndef KWM_T_GRAPH_SHORTEST_PATH_BELLMAN_FORD_NEGATIVE_HPP
#define KWM_T_GRAPH_SHORTEST_PATH_BELLMAN_FORD_NEGATIVE_HPP

#include <vector>
#include <limits>

/**
 * @brief Bellman-Ford（負閉路の影響範囲を含めて検出）
 *
 * 負閉路に到達可能な頂点の距離を -INF にする。
 *
 * 典型用途:
 *   - 負閉路の影響範囲判定
 *   - 「無限に小さくできるか」判定
 *
 * 計算量:
 *   O(VE)
 *
 * 注意:
 *   - 負閉路の影響を受ける頂点は -INF になる
 *
 * verified:
 *   https://atcoder.jp/contests/abc404/submissions/74224294
 */
namespace kwm_t::graph::shortest_path {

	template<typename T>
	std::vector<T> bellman_ford_negative(
		int n,
		const std::vector<std::tuple<int, int, T>> &edges,
		int s,
		T INF,
		bool longest = false
	) {
		std::vector<T> dist(n, INF);
		dist[s] = 0;

		// 通常緩和
		for (int i = 0; i < n - 1; ++i) {
			for (auto[u, v, cost] : edges) {
				T w = longest ? -cost : cost;

				if (dist[u] >= INF) continue;
				if (dist[u] > INF - w) continue;

				if (dist[u] + w < dist[v]) {
					dist[v] = dist[u] + w;
				}
			}
		}

		// 負閉路影響判定
		std::vector<bool> bad(n, false);

		for (int i = 0; i < n; ++i) {
			for (auto[u, v, cost] : edges) {
				T w = longest ? -cost : cost;

				if (dist[u] >= INF) continue;

				// まだ更新できる → 負閉路影響
				if (dist[u] <= INF - w && dist[u] + w < dist[v]) {
					bad[v] = true;
				}

				// 伝播
				if (bad[u]) bad[v] = true;
			}
		}

		// 反映
		for (int i = 0; i < n; ++i) {
			if (bad[i]) dist[i] = -INF;
			if (longest) dist[i] *= -1;
		}

		return dist;
	}

} // namespace kwm_t::graph::shortest_path

#endif // KWM_T_GRAPH_SHORTEST_PATH_BELLMAN_FORD_NEGATIVE_HPP