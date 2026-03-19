#ifndef GRAPH_SHORTEST_PATH_BFS_01_HPP
#define GRAPH_SHORTEST_PATH_BFS_01_HPP

#include <vector>
#include <deque>
#include <utility>
#include <algorithm>

/**
 * @brief 0-1 BFS（辺重みが 0 or 1 の最短路 + 経路復元）
 *
 * 典型用途:
 *   - 重みが {0,1} のグラフ
 *   - グリッド（壁破壊など）
 *
 * 計算量:
 *   O(V + E)
 *
 * @param g 隣接リスト (to, cost) ※ cost は 0 or 1
 * @param s 始点
 * @param inf 無限大
 *
 * @return pair(dist, prev)
 */
namespace graph::shortest_path {

	template<typename T>
	std::pair<std::vector<T>, std::vector<int>>
		bfs_01(const std::vector<std::vector<std::pair<int, T>>> &g, int s, T inf) {
		int n = (int)g.size();

		std::vector<T> dist(n, inf);
		std::vector<int> prev(n, -1);

		std::deque<int> dq;
		dist[s] = 0;
		dq.push_front(s);

		while (!dq.empty()) {
			int v = dq.front();
			dq.pop_front();

			for (auto[to, cost] : g[v]) {
				// cost は 0 or 1 前提
				T nd = dist[v] + cost;
				if (nd < dist[to]) {
					dist[to] = nd;
					prev[to] = v;
					if (cost == 0) dq.push_front(to);
					else dq.push_back(to);
				}
			}
		}

		return { dist, prev };
	}

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

} // namespace graph::shortest_path

#endif // GRAPH_SHORTEST_PATH_BFS_01_HPP