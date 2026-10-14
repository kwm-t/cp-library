/**
 * @file range_edge_graph.hpp
 * @brief 区間辺グラフ
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/range_edge_graph.hpp
 */
#ifndef KWM_T_GRAPH_RANGE_EDGE_GRAPH_HPP
#define KWM_T_GRAPH_RANGE_EDGE_GRAPH_HPP

#include <vector>
#include <queue>
#include <limits>

namespace kwm_t::graph {

/**
 * @brief 区間 → 区間 に辺を張るためのグラフ（セグ木拡張）
 *
 * @tparam W 重み型
 * verified
 *  https://atcoder.jp/contests/abc414/submissions/75096270
 *  https://atcoder.jp/contests/nikkei2019-2-qual/submissions/75096282
 */
template <typename W>
class RangeEdgeGraph {
private:
	struct Edge {
		int to;
		W weight;
	};

	int n, sz;

	// グラフ本体
	std::vector<std::vector<Edge>> g;

	// セグ木ノードの [min, max]
	std::vector<std::pair<W, W>> lr;

	// 元の重み
	std::vector<W> wv;

	static constexpr W INF = std::numeric_limits<W>::max();

public:
	RangeEdgeGraph(int n, std::vector<W> w = {}) : n(n), wv(std::move(w)) {
		sz = 1;
		while (sz < n) sz <<= 1;

		while ((int)wv.size() < sz) wv.push_back(INF);

		g.resize(5 * sz);
		lr.resize(2 * sz);

		// 葉
		for (int i = 0; i < sz; ++i) {
			lr[i + sz] = { wv[i], wv[i] };
		}

		// 内部
		for (int i = sz - 1; i > 0; --i) {
			lr[i].first = lr[i * 2].first;
			lr[i].second = lr[i * 2 + 1].second;
		}

		// セグ木の辺構築
		// type:
		// 0: from-R
		// 1: to-R
		// 2: from-L
		// 3: to-L
		for (int v = 1; v < sz; ++v) {
			for (int w : {v * 2, v * 2 + 1}) {
				g[idx(0, w)].push_back({ idx(0, v), lr[v].second - lr[w].second });
				g[idx(1, v)].push_back({ idx(1, w), lr[w].first - lr[v].first });
				g[idx(2, w)].push_back({ idx(2, v), lr[w].first - lr[v].first });
				g[idx(3, v)].push_back({ idx(3, w), lr[v].second - lr[w].second });
			}
		}
	}

	/**
	 * @brief 区間 [l1, r1) → [l2, r2) に辺を張る
	 */
	void add_edge(int id, int l1, int r1, int l2, int r2, W w) {
		// 超頂点追加
		g.emplace_back();

		if (r1 <= l2) {
			for (int from : divide(l1, r1)) {
				g[idx(0, from)].push_back({
					super(id),
					wv[l2] - lr[from].second + w
					});
			}
			for (int to : divide(l2, r2)) {
				g[super(id)].push_back({
					idx(1, to),
					lr[to].first - wv[l2]
					});
			}
		}
		else {
			for (int from : divide(l1, r1)) {
				g[idx(2, from)].push_back({
					super(id),
					lr[from].first - wv[l1] + w
					});
			}
			for (int to : divide(l2, r2)) {
				g[super(id)].push_back({
					idx(3, to),
					wv[l1] - lr[to].second
					});
			}
		}
	}

	/**
	 * @brief ダイクストラ
	 */
	std::vector<W> dijkstra(int s = 0) const {
		std::vector<W> dist(g.size(), INF);
		dist[s] = 0;

		std::priority_queue<std::pair<W, int>, std::vector<std::pair<W, int>>, std::greater<std::pair<W, int>>> pq;
		pq.emplace(0, s);

		while (!pq.empty()) {
			auto[d, v] = pq.top();
			pq.pop();
			if (d > dist[v]) continue;

			for (const auto& e : g[v]) {
				if (dist[e.to] > dist[v] + e.weight) {
					dist[e.to] = dist[v] + e.weight;
					pq.emplace(dist[e.to], e.to);
				}
			}
		}

		std::vector<W> res(dist.begin(), dist.begin() + n);
		for (auto& x : res) {
			if (x == INF) x = -1;
		}
		return res;
	}

private:
	// type付きノード index
	int idx(int type, int v) const {
		if (v >= sz) return v - sz;
		return v + (type + 1) * sz;
	}

	// 超頂点
	int super(int id) const {
		return 5 * sz + id;
	}

	// 区間分割
	std::vector<int> divide(int l, int r) const {
		std::vector<int> res;
		l += sz;
		r += sz;
		while (l < r) {
			if (l & 1) res.push_back(l++);
			if (r & 1) res.push_back(--r);
			l >>= 1;
			r >>= 1;
		}
		return res;
	}
};

} // namespace kwm_t::graph

#endif