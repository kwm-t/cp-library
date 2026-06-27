#ifndef KWM_T_TREE_HEAVY_LIGHT_DECOMPOSITION_HPP
#define KWM_T_TREE_HEAVY_LIGHT_DECOMPOSITION_HPP

#include <vector>
#include <algorithm>

/**
 * @brief Heavy-Light Decomposition（HLD, 重軽分解）のコア実装
 *
 * 木をパス・部分木クエリに適した区間に分解する。
 * データ構造（Segment Tree / Lazy Segment Tree など）は外部に持たせる設計。
 *
 * 典型用途:
 *   - LCA（Lowest Common Ancestor）
 *   - 木上のパスクエリ（和・最小値・xorなど）
 *   - パス更新（Lazy SegTreeと組み合わせ）
 *   - 部分木クエリ
 *
 * 計算量:
 *   - build: O(N)
 *   - lca / la / dist: O(log N)
 *   - for_each_path: O(log N) 回の区間分割
 *
 * @tparam
 *   特になし（グラフは vector<vector<int>> で受け取る）
 *
 * @param
 *   g: 隣接リスト形式の木（0-indexed）
 *
 * 制約 / 注意:
 *   - 木であること（連結・サイクルなし）
 *   - rootはbuild時に指定可能（デフォルト0）
 *   - 非可換演算の場合は、for_each_pathの順序に注意して自分で管理すること
 *   - 辺クエリは「子ノードに値を持たせる」ことで対応
 *
 * 使用例:
 *   HLD hld(g);
 *   hld.build();
 *
 *   // パスクエリ
 *   long long res = 0;
 *   hld.for_each_path(u, v, [&](int l, int r) {
 *       res += seg.prod(l, r);
 *   });
 *
 * verified:
 *   - https://judge.yosupo.jp/submission/365641
 *   - https://atcoder.jp/contests/abc294/submissions/74815633
 *   - https://atcoder.jp/contests/abc406/submissions/74815734
 *   - https://atcoder.jp/contests/pakencamp-2022-day1/submissions/74815849
 *   - https://yukicoder.me/submissions/1157023
 *   - https://yukicoder.me/submissions/1157028
 *   - https://judge.yosupo.jp/submission/365677
 *   - 辺に情報を乗せる頂点倍+遅延セグ木
 *   - https://atcoder.jp/contests/awc0100/submissions/76970430
 *   - 辺に情報を乗せる+遅延セグ木
 *   - https://atcoder.jp/contests/awc0100/submissions/76977123
 */
namespace kwm_t::tree {

class HeavyLightDecomposition {
private:
	int n;
	std::vector<std::vector<int>> g;

	std::vector<int> sz, par, depth;
	std::vector<int> in, out, head, rev;

	void dfs_sz(int v, int p) {
		par[v] = p;
		sz[v] = 1;
		for (auto& to : g[v]) {
			if (to == p) continue;
			depth[to] = depth[v] + 1;
			dfs_sz(to, v);
			sz[v] += sz[to];
			if (sz[to] > sz[g[v][0]] || g[v][0] == p) {
				std::swap(to, g[v][0]);
			}
		}
	}

	void dfs_hld(int v, int p, int& t) {
		in[v] = t;
		rev[t] = v;
		++t;
		for (auto to : g[v]) {
			if (to == p) continue;
			head[to] = (to == g[v][0] ? head[v] : to);
			dfs_hld(to, v, t);
		}
		out[v] = t;
	}

public:
	HeavyLightDecomposition(const std::vector<std::vector<int>>& graph)
		: n(graph.size()), g(graph),
		sz(n), par(n), depth(n),
		in(n), out(n), head(n), rev(n) {}

	void build(int root = 0) {
		depth[root] = 0;
		dfs_sz(root, -1);
		int t = 0;
		head[root] = root;
		dfs_hld(root, -1, t);
	}

	// LCA
	int lca(int u, int v) const {
		while (head[u] != head[v]) {
			if (depth[head[u]] > depth[head[v]]) u = par[head[u]];
			else v = par[head[v]];
		}
		return (depth[u] < depth[v] ? u : v);
	}

	// k-th ancestor
	int la(int v, int k) const {
		while (true) {
			int h = head[v];
			if (in[v] - k >= in[h]) return rev[in[v] - k];
			k -= (in[v] - in[h] + 1);
			v = par[h];
		}
	}

	int dist(int u, int v) const {
		int w = lca(u, v);
		return depth[u] + depth[v] - 2 * depth[w];
	}

	// ===== 頂点パス =====
	// 区間 [l, r) をコールバックに渡す
	template <class F>
	void for_each_path(int u, int v, const F& f) const {
		while (head[u] != head[v]) {
			if (depth[head[u]] > depth[head[v]]) {
				f(in[head[u]], in[u] + 1, true);
				u = par[head[u]];
			}
			else {
				f(in[head[v]], in[v] + 1, false);
				v = par[head[v]];
			}
		}
		if (depth[u] > depth[v]) f(in[v], in[u] + 1, true);
		else f(in[u], in[v] + 1, false);
	}

	// ===== 辺パス =====
    // 非可換の場合のverifyがされていません。
	template <class F>
	void for_each_path_edge(int u, int v, const F& f) const {
		while (head[u] != head[v]) {
			if (depth[head[u]] > depth[head[v]]) {
				f(in[head[u]], in[u] + 1, true);
				u = par[head[u]];
			}
			else {
				f(in[head[v]], in[v] + 1, false);
				v = par[head[v]];
			}
		}
		if (u == v) return;
		if (depth[u] > depth[v]) f(in[v] + 1, in[u] + 1, true);
		else f(in[u] + 1, in[v] + 1, false);
	}

	// ===== 部分木 =====
	template <class F>
	void for_each_subtree(int v, const F& f) const {
		f(in[v], out[v]);
	}

	// ===== getter =====
	int get_in(int v) const { return in[v]; }
	int get_out(int v) const { return out[v]; }
	int get_rev(int i) const { return rev[i]; }
	int get_par(int v) const { return par[v]; }
	int get_depth(int v) const { return depth[v]; }
};

}

#endif // KWM_T_TREE_HEAVY_LIGHT_DECOMPOSITION_HPP