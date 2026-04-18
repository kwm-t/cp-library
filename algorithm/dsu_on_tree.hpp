#ifndef KWM_T_ALGORITHM_DSU_ON_TREE_HPP
#define KWM_T_ALGORITHM_DSU_ON_TREE_HPP

#include <vector>
#include <utility>

/**
 * @brief DSU on Tree (Sack Technique)
 *
 * @details
 *  木上で部分木クエリを高速に処理するテクニック。
 *  Heavy Child を残し、Small Child を都度破棄することで
 *  計算量を削減する。
 *
 *  Euler Tour により部分木を区間 [from[u], to[u]) に対応させる。
 *
 *  提供機能:
 *    - run(): 各頂点の部分木に対するクエリ
 *    - run_every_pair(): 全頂点ペアに関するクエリ
 *
 *  計算量:
 *    add/erase:O(NlogN)回
 *    query/reset:N回
 *
 *  verified:
 *    https://yukicoder.me/submissions/1160260
 *    https://atcoder.jp/contests/abc454/submissions/75095569
 */
namespace kwm_t::algorithm {

class DsuOnTree {
private:
	std::vector<std::vector<int>> graph;
	int n;
	int root;

	std::vector<int> sub;
	std::vector<int> ord;
	std::vector<int> from;
	std::vector<int> to;

	int dfs_sub(int v, int p) {
		sub[v] = 1;

		if (graph[v].size() >= 2 && graph[v][0] == p) {
			std::swap(graph[v][0], graph[v][1]);
		}

		for (int i = 0; i < (int)graph[v].size(); i++) {
			int u = graph[v][i];
			if (u == p) continue;

			sub[v] += dfs_sub(u, v);

			if (i != 0 && sub[graph[v][0]] < sub[u]) {
				std::swap(graph[v][0], graph[v][i]);
			}
		}

		return sub[v];
	}

	void dfs_ord(int v, int p, int& idx) {
		from[v] = idx;
		ord[idx++] = v;

		for (int u : graph[v]) {
			if (u == p) continue;
			dfs_ord(u, v, idx);
		}

		to[v] = idx;
	}

public:
	DsuOnTree(const std::vector<std::vector<int>>& g, int r)
		: graph(g), n((int)g.size()), root(r),
		sub(n), ord(n), from(n), to(n) {
		dfs_sub(root, -1);
		int idx = 0;
		dfs_ord(root, -1, idx);
	}

	template <typename Update, typename Query, typename Clear, typename Reset>
	void run(Update& update, Query& query, Clear& clear, Reset& reset) {
		auto dfs = [&](auto&& self, int v, int p, bool keep) -> void {
			for (int i = 1; i < (int)graph[v].size(); i++) {
				int u = graph[v][i];
				if (u == p) continue;
				self(self, u, v, false);
			}

			if (sub[v] > 1) {
				self(self, graph[v][0], v, true);
			}

			if (sub[v] > 1) {
				int heavy = graph[v][0];
				for (int i = to[heavy]; i < to[v]; i++) {
					update(ord[i]);
				}
			}

			update(v);
			query(v);

			if (!keep) {
				for (int i = from[v]; i < to[v]; i++) {
					clear(ord[i]);
				}
				reset();
			}
		};

		dfs(dfs, root, -1, false);
	}

	template <typename Update, typename Query_Root, typename Query_Light, typename Clear, typename Reset>
	void run_every_pair(Update& update, Query_Root& query_root, Query_Light& query_light, Clear& clear, Reset& reset) {

		auto dfs = [&](auto&& self, int v, int p, bool keep) -> void {
			for (int i = 1; i < (int)graph[v].size(); i++) {
				int u = graph[v][i];
				if (u == p) continue;
				self(self, u, v, false);
			}

			if (sub[v] > 1) {
				self(self, graph[v][0], v, true);
			}

			if (sub[v] > 1) {
				for (int i = 1; i < (int)graph[v].size(); i++) {
					int ch = graph[v][i];
					if (ch == p) continue;

					for (int j = from[ch]; j < to[ch]; j++) {
						query_light(v, ord[j]);
					}
					for (int j = from[ch]; j < to[ch]; j++) {
						update(ord[j]);
					}
				}
			}

			update(v);
			query_root(v);

			if (!keep) {
				for (int i = from[v]; i < to[v]; i++) {
					clear(ord[i]);
				}
				reset();
			}
		};

		dfs(dfs, root, -1, false);
	}
};

} // namespace kwm_t::algorithm

#endif // KWM_T_ALGORITHM_DSU_ON_TREE_HPP