/**
 * @file johnson.hpp
 * @brief Johnson 法による全点対最短路
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/shortest_path/johnson.hpp
 */
#ifndef KWM_T_GRAPH_SHORTEST_PATH_JOHNSON_HPP
#define KWM_T_GRAPH_SHORTEST_PATH_JOHNSON_HPP

#include <vector>
#include <utility>

#include "dijkstra.hpp"

/**
 * @brief Johnson's Algorithm（負辺対応・全点対最短路）
 *
 * 負辺を含む有向グラフに対して、全頂点対の最短距離を求める。
 * 負閉路が存在しないことを仮定する。
 *
 * 全頂点を初期距離 0 とした Bellman-Ford 法でポテンシャルを計算し、
 * 各辺を非負重みに変換した後、各頂点から Dijkstra 法を実行する。
 *
 * 典型用途:
 *   - 負辺を含むグラフの全点対最短路
 *   - 疎グラフの全点対最短路
 *
 * 計算量:
 *   O(VE + V(V + E) log V)
 *
 * @tparam T 距離・辺重みの型
 *
 * @param g 隣接リスト (to, cost)
 * @param inf 無限大
 *
 * @return dist[i][j]
 *   頂点 i から頂点 j への最短距離。
 *   到達不能な場合は inf。
 *
 * 制約 / 注意:
 *   - 負閉路が存在しないこと
 *   - inf は十分大きな値を指定すること
 *
 * 使用例:
 *   using T = long long;
 *
 *   std::vector<std::vector<std::pair<int, T>>> g(n);
 *   g[0].emplace_back(1, -5);
 *   g[1].emplace_back(2, 10);
 *
 *   auto dist = johnson(g, INF);
 *
 * verified:
 *   https://yukicoder.me/submissions/1180223
 */
namespace kwm_t::graph::shortest_path {

template<typename T>
std::vector<std::vector<T>> johnson(const std::vector<std::vector<std::pair<int, T>>>& g, T inf) {

	const int n = (int)g.size();

	// 1. ポテンシャルの計算
	// 超頂点から全頂点へ重み 0 の辺を張る代わりに、
	// 全頂点の初期距離を 0 として Bellman-Ford を行う。
	std::vector<T> h(n, 0);
	for (int i = 0; i < n - 1; ++i) {
		bool updated = false;
		for (int u = 0; u < n; ++u) {
			for (auto [v, cost] : g[u]) {
				if (h[u] + cost < h[v]) {
					h[v] = h[u] + cost;
					updated = true;
				}
			}
		}
		if (!updated) break;
	}

	// 2. 辺の重みを非負化
	// new_cost = cost + h[u] - h[v]
	std::vector<std::vector<std::pair<int, T>>> reweighted_g(n);

	for (int u = 0; u < n; ++u) {
		reweighted_g[u].reserve(g[u].size());

		for (auto [v, cost] : g[u]) {
			T new_cost = cost + h[u] - h[v];
			reweighted_g[u].emplace_back(v, new_cost);
		}
	}

	// 3. 各頂点から Dijkstra
	std::vector<std::vector<T>> dist(n, std::vector<T>(n, inf));

	for (int s = 0; s < n; ++s) {
		auto [d, prev] = dijkstra(reweighted_g, s, inf);

		for (int t = 0; t < n; ++t) {
			if (d[t] >= inf) continue;

			// 元の距離へ復元
			dist[s][t] = d[t] - h[s] + h[t];
		}
	}
	return dist;
}

} // namespace kwm_t::graph::shortest_path

#endif // KWM_T_GRAPH_SHORTEST_PATH_JOHNSON_HPP