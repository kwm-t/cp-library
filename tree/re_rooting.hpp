/**
 * @file re_rooting.hpp
 * @brief 全方位木 DP（Re-rooting）
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/tree/re_rooting.hpp
 */
#ifndef KWM_T_TREE_REROOTING_HPP
#define KWM_T_TREE_REROOTING_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::tree {

/**
 * @brief ReRooting DP (全方位木DP)
 *
 * 典型用途:
 *   - 各頂点を根としたDP結果
 *   - 木の全方位問題（距離和、部分木最適化など）
 *
 * 計算量:
 *   O(N * merge)
 *
 * verified:
 *   https://atcoder.jp/contests/arc179/submissions/74823918
 *   https://atcoder.jp/contests/abc394/submissions/74823955
 */
template <
	typename Data,
	Data(*e)(),
	Data(*addEdge)(const Data&, int),
	Data(*addVertex)(const Data&, int),
	Data(*merge)(const Data&, const Data&)
>
class ReRooting {
public:
	struct Edge {
		int from, to;
		int index;
	};

	int n;
	std::vector<std::vector<Edge>> g;
	std::vector<Data> sub, dp;

public:
	explicit ReRooting(int n) : n(n), g(n) {}

	void add_edge(int u, int v, int idx = 0) {
		g[u].push_back({ u, v, idx });
		g[v].push_back({ v, u, idx });
	}

	std::vector<Data> build() {
		sub.assign(n, e());
		dfs1(0, -1);
		dp = sub;
		dfs2(0, -1);
		return dp;
	}

private:
	void dfs1(int v, int p) {
		Data res = e();

		for (auto edge : g[v]) {
			if (edge.to == p) continue;
			dfs1(edge.to, v);
			res = merge(res, addEdge(sub[edge.to], edge.index));
		}

		sub[v] = addVertex(res, v);
	}

	void dfs2(int v, int p) {
		std::vector<Data>vals;
		for (Edge &edge : g[v]) {
			vals.push_back(addEdge(sub[edge.to], edge.index));
		}
		int m = g[v].size();

		std::vector<Data> pref(m + 1, e()), suf(m + 1, e());

		for (int i = 0; i < m; i++) {
			pref[i + 1] = merge(pref[i], vals[i]);
		}
		for (int i = m - 1; i >= 0; i--) {
			suf[i] = merge(suf[i + 1], vals[i]);
		}

		dp[v] = addVertex(pref[m], v);

		for (int i = 0; i < m; i++) {
			int to = g[v][i].to;
			if (to == p) continue;

			sub[v] = addVertex(merge(pref[i], suf[i + 1]), v);
			dfs2(to, v);
		}
	}
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_REROOTING_HPP