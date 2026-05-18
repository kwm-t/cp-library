#ifndef KWM_T_GRAPH_BIPARTITE_COLORING_HPP
#define KWM_T_GRAPH_BIPARTITE_COLORING_HPP

#include <vector>

namespace kwm_t::graph::bipartite {
/**
 * @brief 二部グラフ判定 + 2色塗り
 *
 * 典型用途:
 *   - グラフが二部グラフか判定
 *   - 頂点を 0/1 に色分け
 *   - 連結成分数の取得
 *
 * 計算量:
 *   - O(N + M)
 *
 * @param n 頂点数
 *
 * 制約 / 注意:
 *   - 無向グラフを想定
 *
 * 使用例:
 *   BipartiteColoring bc(n);
 *   bc.add_edge(u, v);
 *   bool ok = bc.is_bipartite();
 *   auto col = bc.color; // 0 or 1
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0071/submissions/75933094
 */
class BipartiteColoring {
public:
	explicit BipartiteColoring(int n, const std::vector<std::vector<int>>& graph)
		: n(n), graph(graph), color(n, -1), components(0) {
		execute();
	}

	int get_components() const {
		return components;
	}

	const std::vector<int>& get_color() const {
		return color;
	}

private:
	int n;
	bool is_bipartite;
	std::vector<std::vector<int>> graph;
	std::vector<int> color;
	int components;

	bool dfs(int v) {
		bool ok = true;
		for (int nv : graph[v]) {
			if (color[nv] != -1) {
				if (color[nv] == color[v]) ok = false;
			}
			else {
				color[nv] = 1 - color[v];
				if (!dfs(nv)) ok = false;
			}
		}
		return ok;
	}

	bool execute() {
		bool ok = true;
		components = 0;

		for (int i = 0; i < n; ++i) {
			if (color[i] != -1) continue;
			color[i] = 0;
			components++;
			if (!dfs(i)) ok = false;
		}

		return ok;
	}
};

} // namespace kwm_t::graph::bipartite

#endif // KWM_T_GRAPH_BIPARTITE_COLORING_HPP