#ifndef KWM_T_GRAPH_SHORTEST_PATH_GRID_BFS_HPP
#define KWM_T_GRAPH_SHORTEST_PATH_GRID_BFS_HPP

#include <vector>
#include <queue>
#include <utility>
#include <algorithm>

/**
 * @brief Grid BFS（4近傍・障害物対応・経路復元）
 *
 * 典型用途:
 *   - 迷路最短路
 *   - グリッド探索
 *
 * 計算量:
 *   O(HW)
 *
 * @param grid '.' = 通行可, '#' = 壁（想定）
 * @param sx, sy 始点
 *
 * @return dist, prev（1次元 index）
 */
namespace kwm_t::graph::shortest_path {

	struct GridBFSResult {
		std::vector<int> dist;
		std::vector<int> prev;
		int h, w;

		int id(int x, int y) const { return x * w + y; }

		std::pair<int, int> pos(int v) const {
			return { v / w, v % w };
		}
	};

	inline GridBFSResult
		grid_bfs(const std::vector<std::string> &grid, int sx, int sy) {
		int h = (int)grid.size();
		int w = (int)grid[0].size();
		int n = h * w;

		std::vector<int> dist(n, -1);
		std::vector<int> prev(n, -1);

		auto id = [&](int x, int y) { return x * w + y; };

		std::queue<int> q;
		dist[id(sx, sy)] = 0;
		q.push(id(sx, sy));

		const int dx[4] = { 1, -1, 0, 0 };
		const int dy[4] = { 0, 0, 1, -1 };

		while (!q.empty()) {
			int v = q.front(); q.pop();
			int x = v / w, y = v % w;

			for (int dir = 0; dir < 4; ++dir) {
				int nx = x + dx[dir];
				int ny = y + dy[dir];

				if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
				if (grid[nx][ny] == '#') continue;

				int to = id(nx, ny);
				if (dist[to] != -1) continue;

				dist[to] = dist[v] + 1;
				prev[to] = v;
				q.push(to);
			}
		}

		return { dist, prev, h, w };
	}

	inline std::vector<std::pair<int, int>>
		restore_path(const GridBFSResult &res, int sx, int sy, int tx, int ty) {
		std::vector<std::pair<int, int>> path;

		int s = res.id(sx, sy);
		int t = res.id(tx, ty);

		for (int v = t; v != -1; v = res.prev[v]) {
			path.push_back(res.pos(v));
		}
		std::reverse(path.begin(), path.end());

		if (path.empty() || path[0] != std::make_pair(sx, sy)) return {};
		return path;
	}

} // namespace kwm_t::graph::shortest_path

#endif // KWM_T_GRAPH_SHORTEST_PATH_GRID_BFS_HPP