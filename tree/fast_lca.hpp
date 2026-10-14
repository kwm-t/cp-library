/**
 * @file fast_lca.hpp
 * @brief 高速 LCA
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/tree/fast_lca.hpp
 */
#ifndef KWM_T_TREE_FAST_LCA_HPP
#define KWM_T_TREE_FAST_LCA_HPP

#include <vector>
#include <numeric>
#include <algorithm>

namespace kwm_t::tree {

/**
 * @brief Euler Tour + Sparse Table による高速LCA
 *
 * 典型用途:
 *   - LCA
 *   - 木上距離
 *
 * 計算量:
 *   - 前処理: O(N log N)
 *   - LCA: O(1)
 *   - dist: O(1)
 *
 * verified:
 *   - https://atcoder.jp/contests/abc460/submissions/76290187
 */
class FastLca {
private:
	std::vector<int> tour;
	std::vector<int> depth_tour;
	std::vector<int> first;
	std::vector<int> depth;

	std::vector<int> lg;
	std::vector<std::vector<int>> st;

public:
	FastLca() = default;

	FastLca(const std::vector<std::vector<int>>& g, int root = 0) {
		build(g, root);
	}

	void build(const std::vector<std::vector<int>>& g, int root = 0) {
		int n = (int)g.size();

		tour.clear();
		depth_tour.clear();

		first.assign(n, -1);
		depth.assign(n, 0);

		tour.reserve(2 * n - 1);
		depth_tour.reserve(2 * n - 1);

		auto dfs = [&](auto&& self, int v, int p, int d) -> void {
			depth[v] = d;

			if (first[v] == -1) {
				first[v] = (int)tour.size();
			}

			tour.push_back(v);
			depth_tour.push_back(d);

			for (int to : g[v]) {
				if (to == p) continue;

				self(self, to, v, d + 1);

				tour.push_back(v);
				depth_tour.push_back(d);
			}
		};

		dfs(dfs, root, -1, 0);

		int m = (int)tour.size();

		lg.assign(m + 1, 0);
		for (int i = 2; i <= m; i++) {
			lg[i] = lg[i >> 1] + 1;
		}

		int K = lg[m] + 1;

		st.assign(K, std::vector<int>(m));

		std::iota(st[0].begin(), st[0].end(), 0);

		for (int k = 1; k < K; k++) {
			int len = 1 << k;

			for (int i = 0; i + len <= m; i++) {
				int a = st[k - 1][i];
				int b = st[k - 1][i + (len >> 1)];

				st[k][i] =
					(depth_tour[a] < depth_tour[b])
					? a
					: b;
			}
		}
	}

	/**
	 * @brief Lowest Common Ancestor
	 */
	int lca(int u, int v) const {
		int l = first[u];
		int r = first[v];

		if (l > r) std::swap(l, r);

		int k = lg[r - l + 1];

		int a = st[k][l];
		int b = st[k][r - (1 << k) + 1];

		int idx =
			(depth_tour[a] < depth_tour[b])
			? a
			: b;

		return tour[idx];
	}

	/**
	 * @brief 木上距離
	 */
	int dist(int u, int v) const {
		int p = lca(u, v);
		return depth[u] + depth[v] - depth[p] * 2;
	}

	/**
	 * @brief 頂点の深さ
	 */
	int get_depth(int v) const {
		return depth[v];
	}
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_FAST_LCA_HPP