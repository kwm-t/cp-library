/**
 * @file centroid_decomposition.hpp
 * @brief 重心分解
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/tree/centroid_decomposition.hpp
 */
#ifndef KWM_T_TREE_CENTROID_DECOMPOSITION_HPP
#define KWM_T_TREE_CENTROID_DECOMPOSITION_HPP

#include <vector>

namespace kwm_t::tree {

/**
 * @brief 重心分解 (Centroid Decomposition)
 *
 * 典型用途:
 *  - 木の分割統治
 *  - 距離制約クエリ
 *  - パス・部分木に関する高速処理
 *
 * 計算量:
 *  - 構築: O(N log N)
 *
 * @param 木（無向・連結・N-1辺）
 *
 * 制約 / 注意:
 *  - 0-indexed
 *  - build() を呼ぶと par に重心木が構築される
 *
 * 使用例:
 *  CentroidDecomposition cd(n);
 *  cd.add_edge(u, v);
 *  int root = cd.build();
 *
 * verified:
 *  https://atcoder.jp/contests/abc291/submissions/74909638
 */
class CentroidDecomposition {
private:
	int n;
	std::vector<std::vector<int>> g;
	std::vector<bool> used;
	std::vector<int> sz, par;

	void calc_size(int v, int p) {
		sz[v] = 1;
		for (int to : g[v]) {
			if (to == p || used[to]) continue;
			calc_size(to, v);
			sz[v] += sz[to];
		}
	}

	int find_centroid(int v) {
		calc_size(v, -1);
		int total = sz[v];

		int p = -1;
		bool moved = true;
		while (moved) {
			moved = false;
			for (int to : g[v]) {
				if (to == p || used[to]) continue;
				if (sz[to] * 2 > total) {
					p = v;
					v = to;
					moved = true;
					break;
				}
			}
		}
		return v;
	}

	template <class F>
	void decompose(int v, int p, F& f) {
		int c = find_centroid(v);
		par[c] = p;

		f(c);

		used[c] = true;

		for (int to : g[c]) {
			if (!used[to]) {
				decompose(to, c, f);
			}
		}
	}

public:
	explicit CentroidDecomposition(int n)
		: n(n), g(n), used(n, false), sz(n), par(n, -1) {}

	void add_edge(int u, int v) {
		g[u].push_back(v);
		g[v].push_back(u);
	}

	void build() {
		auto dummy = [](int) {};
		decompose(0, -1, dummy);
	}

	template <class F>
	void build(F f) {
		decompose(0, -1, f);
	}

	const std::vector<int>& parent() const {
		return par;
	}

	const std::vector<std::vector<int>>& graph() const {
		return g;
	}

	const std::vector<bool>& is_used() const {
		return used;
	}
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_CENTROID_DECOMPOSITION_HPP