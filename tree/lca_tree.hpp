#ifndef KWM_T_TREE_LCA_TREE_HPP
#define KWM_T_TREE_LCA_TREE_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::tree {

/**
 * @brief LCA (Binary Lifting) + 距離 + k祖先 + パス判定
 *
 * 典型用途:
 *   - LCA
 *   - 木上の距離
 *   - k-th ancestor
 *   - パス上判定 / ジャンプ
 *
 * 計算量:
 *   - 前処理: O(N log N)
 *   - LCA / k-th ancestor: O(log N)
 *
 * verified:
 *  - https://atcoder.jp/contests/awc0059/submissions/75370597
 */
class LcaTree {
public:
	static const int MAXLOGV = 25;

	int n, root;
	std::vector<std::vector<int>> g;

	std::vector<int> depth;
	std::vector<int> in, out;
	std::vector<int> parent[MAXLOGV];

public:
	LcaTree(std::vector<std::vector<int>> g, int root = 0)
		: n((int)g.size()), root(root),
		g(g) {
		build();
	}

	// LCA
	int lca(int u, int v) const {
		if (depth[u] > depth[v]) std::swap(u, v);

		int diff = depth[v] - depth[u];
		for (int k = 0; k < MAXLOGV; k++) {
			if (diff >> k & 1) {
				v = parent[k][v];
			}
		}

		if (u == v) return u;

		for (int k = MAXLOGV - 1; k >= 0; k--) {
			if (parent[k][u] != parent[k][v]) {
				u = parent[k][u];
				v = parent[k][v];
			}
		}

		return parent[0][u];
	}

	// k個上の祖先
	int kth_ancestor(int v, int k) const {
		if (depth[v] < k) return -1;

		for (int i = 0; i < MAXLOGV; i++) {
			if (k >> i & 1) {
				v = parent[i][v];
				if (v == -1) return -1;
			}
		}
		return v;
	}

	// 距離
	int dist(int u, int v) const {
		int p = lca(u, v);
		return (depth[u] - depth[p]) + (depth[v] - depth[p]);
	}

	// パス上判定
	bool on_path(int u, int v, int w) const {
		return dist(u, w) + dist(v, w) == dist(u, v);
	}

	// uからv方向へd進むノード
	int jump(int u, int v, int d) const {
		if (d == 0) return u;

		int uv = dist(u, v);
		if (uv < d) return -1;

		int p = lca(u, v);

		if (p == u) {
			return kth_ancestor(v, uv - d);
		}
		if (p == v) {
			return kth_ancestor(u, d);
		}

		int up = depth[u] - depth[p];
		if (up >= d) {
			return kth_ancestor(u, d);
		}
		return kth_ancestor(v, uv - d);
	}

private:
	// 前処理
	void build() {
		depth.resize(n);
		in.resize(n);
		out.resize(n);
		for (int i = 0; i < MAXLOGV; i++) {
			parent[i].assign(n, -1);
		}

		int timer = 0;

		auto dfs = [&](auto&& self, int v, int p, int d) -> void {
			in[v] = timer++;
			parent[0][v] = p;
			depth[v] = d;

			for (int to : g[v]) {
				if (to == p) continue;
				self(self, to, v, d + 1);
			}

			out[v] = timer;
		};

		dfs(dfs, root, -1, 0);

		for (int k = 0; k + 1 < MAXLOGV; k++) {
			for (int v = 0; v < n; v++) {
				if (parent[k][v] < 0) {
					parent[k + 1][v] = -1;
				}
				else {
					parent[k + 1][v] = parent[k][parent[k][v]];
				}
			}
		}
	}
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_LCA_TREE_HPP