/**
 * @file low_link.hpp
 * @brief Lowlink
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/low_link.hpp
 */
#ifndef KWM_T_GRAPH_LOWLINK_HPP
#define KWM_T_GRAPH_LOWLINK_HPP

#include <vector>
#include <algorithm>
#include <iostream>

/**
 * @brief Lowlink（関節点・橋・2点間の橋必須判定）
 *
 * 典型用途:
 *   - 関節点（articulation points）の列挙
 *   - 橋（bridge）の列挙
 *   - 辺が s-t パスに必ず含まれるか判定
 *
 * 計算量:
 *   - 構築: O(N + M)
 *   - クエリ: O(1)
 *
 * @param グラフは (to, edge_id) を持つ隣接リスト
 *
 * 制約 / 注意:
 *   - 無向グラフで使用すること
 *   - edge_id は 0..M-1 の範囲であること
 *
 * 使用例:
 *   Lowlink ll(G, M);
 *   ll.build();
 *   auto aps = ll.articulation_points;
 *
 * verified:
 *   - https://yukicoder.me/submissions/1157134
 *   - https://yukicoder.me/submissions/1157136
 *   - https://atcoder.jp/contests/abc334/submissions/74910556
 *   - https://atcoder.jp/contests/abc375/submissions/74910599
 */
namespace kwm_t::graph {

class Lowlink {
private:
	using Edge = std::vector<std::vector<std::pair<int, int>>>;

public:
	int sz;
	int m;
	Edge G;

	// 関節点
	std::vector<int> articulation_points;

	// 橋 (edge_id -> 子頂点, -1なら橋でない)
	std::vector<int> bridge;

	// DFS木の根
	std::vector<bool> is_root;

private:
	std::vector<int> in, out, low;
	std::vector<std::vector<int>> child;

public:
	Lowlink(const Edge& G, int m) : G(G), m(m) {}

	void build() {
		sz = G.size();

		in.assign(sz, -1);
		out.assign(sz, -1);
		low.assign(sz, -1);
		child.assign(sz, {});
		is_root.assign(sz, false);

		bridge.assign(m, -1);

		int k = 0;
		for (int i = 0; i < sz; ++i) {
			if (in[i] < 0) {
				is_root[i] = true;
				dfs(i, -1, k);
			}
		}
	}

	/**
	 * @brief 頂点 v を削除したときの連結成分数
	 */
	int remove(int v) const {
		int c = 0;
		for (int nv : child[v]) {
			if (in[v] <= low[nv]) c++;
		}
		if (!is_root[v]) c++;
		return c;
	}

	/**
	 * @brief 辺 a が s-t パスに必ず含まれるか
	 */
	bool query(int a, int s, int t) const {
		if (bridge[a] == -1) return false;

		int l = in[bridge[a]];
		int r = out[bridge[a]];

		s = in[s];
		t = in[t];

		return ((l <= s && s < r) ^ (l <= t && t < r));
	}
private:
	void dfs(int v, int p_edge, int& k) {
		in[v] = low[v] = k++;
		bool is_art = false;

		for (auto[nv, id] : G[v]) {
			if (in[nv] < 0) {
				child[v].push_back(nv);
				dfs(nv, id, k);

				low[v] = std::min(low[v], low[nv]);

				// 関節点判定
				if (p_edge != -1 && in[v] <= low[nv]) {
					is_art = true;
				}

				// 橋判定
				if (in[v] < low[nv]) {
					bridge[id] = nv;
				}
			}
			else if (id != p_edge) {
				low[v] = std::min(low[v], in[nv]);
			}
		}

		// 根の場合
		if (p_edge == -1 && child[v].size() >= 2) {
			is_art = true;
		}

		if (is_art) {
			articulation_points.push_back(v);
		}

		out[v] = k++;
	}
};

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_LOWLINK_HPP