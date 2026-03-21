#ifndef KWM_T_GRAPH_SHORTEST_PATH_DIJKSTRA_HPP
#define KWM_T_GRAPH_SHORTEST_PATH_DIJKSTRA_HPP

#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <functional>
#include <algorithm>

/**
 * @brief ダイクストラ法（単一始点最短路 + 経路復元用情報）
 *
 * 非負重みグラフにおける単一始点最短路を求める。
 * prev を用いることで最短経路の復元が可能。
 *
 * 典型用途:
 *   - 重み付きグラフ（非負）
 *   - グリッド + コスト
 *   - 最短経路 + 経路復元
 *
 * 計算量:
 *   O((V + E) log V)
 *
 * @tparam T 距離型（int, long long など）
 *
 * @param g 隣接リスト (to, cost)
 * @param s 始点
 * @param inf 無限大
 *
 * @return pair(dist, prev)
 *
 * 制約 / 注意:
 *   - 負辺がある場合は使用不可（Bellman-Fordを使う）
 *
 * 使用例:
 *   auto [dist, prev] = dijkstra(g, s, INF);
 *   auto path = restore_path(prev, s, t);
 *
 * verified:
 *   - AtCoder ABC / typical90
 */
namespace kwm_t::graph::shortest_path {

	template<typename T>
	std::pair<std::vector<T>, std::vector<int>>
		dijkstra(const std::vector<std::vector<std::pair<int, T>>> &g, int s, T inf) {
		int n = (int)g.size();

		std::vector<T> dist(n, inf);
		std::vector<int> prev(n, -1);

		using P = std::pair<T, int>;
		std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

		dist[s] = 0;
		pq.emplace(0, s);

		while (!pq.empty()) {
			auto[d, v] = pq.top();
			pq.pop();

			if (d > dist[v]) continue;

			for (auto[to, cost] : g[v]) {
				if (dist[v] > inf - cost) continue; // overflow guard

				T nd = dist[v] + cost;
				if (nd < dist[to]) {
					dist[to] = nd;
					prev[to] = v;
					pq.emplace(nd, to);
				}
			}
		}

		return { dist, prev };
	}

	/**
	 * @brief 経路復元
	 * @param prev dijkstra で得た prev
	 * @param s 始点
	 * @param t 終点
	 * @return s -> t の最短経路（存在しない場合は空）
	 */
	inline std::vector<int>
		restore_path(const std::vector<int> &prev, int s, int t) {
		std::vector<int> path;
		for (int v = t; v != -1; v = prev[v]) {
			path.push_back(v);
		}
		std::reverse(path.begin(), path.end());

		if (path.empty() || path[0] != s) return {};
		return path;
	}

} // namespace kwm_t::graph::shortest_path

#endif // KWM_T_GRAPH_SHORTEST_PATH_DIJKSTRA_HPP