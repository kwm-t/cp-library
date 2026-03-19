#ifndef GRAPH_SHORTEST_PATH_BFS_HPP
#define GRAPH_SHORTEST_PATH_BFS_HPP

#include <vector>
#include <queue>
#include <algorithm>

/**
 * @brief BFS（無重みグラフの最短距離 + 経路復元）
 *
 * 典型用途:
 *   - 無重みグラフの最短路
 *   - 木の距離
 *
 * 計算量:
 *   O(V + E)
 *
 * @param g 隣接リスト
 * @param s 始点
 *
 * @return pair(dist, prev)
 */
namespace graph::shortest_path {

	inline std::pair<std::vector<int>, std::vector<int>>
		bfs(const std::vector<std::vector<int>> &g, int s) {
		int n = (int)g.size();
		std::vector<int> dist(n, -1);
		std::vector<int> prev(n, -1);

		std::queue<int> q;
		dist[s] = 0;
		q.push(s);

		while (!q.empty()) {
			int v = q.front(); q.pop();
			for (int to : g[v]) {
				if (dist[to] != -1) continue;
				dist[to] = dist[v] + 1;
				prev[to] = v;
				q.push(to);
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

#endif // GRAPH_SHORTEST_PATH_BFS_HPP