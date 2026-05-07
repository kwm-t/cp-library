#ifndef KWM_T_GRAPH_KRUSKAL_HPP
#define KWM_T_GRAPH_KRUSKAL_HPP

#include <vector>
#include <algorithm>

#include <atcoder/dsu>

#include "edge.hpp"

namespace kwm_t::graph {

/**
 * @brief 最小全域木の結果
 *
 * @tparam Cost
 *   辺コスト型
 *
 * @tparam Sum
 *   コスト総和型
 */
template<class Cost, class Sum = long long>
struct MinimumSpanningTree {
	Sum cost;
	std::vector<Edge<Cost>> edges;
	bool connected;
};

/**
 * @brief Kruskal 法で最小全域木を求める
 *
 * 無向グラフの MST を構築する。
 *
 * 戻り値の edges には
 * 「採用された辺」
 * が格納される。
 *
 * 計算量:
 *   O(M log M)
 *
 * @tparam Cost
 *   辺コスト型
 *
 * @tparam Sum
 *   コスト総和型
 *
 * @param n
 *   頂点数
 *
 * @param edges
 *   辺集合
 *
 * @return MinimumSpanningTree<Cost, Sum>
 *
 * 制約 / 注意:
 * - 無向グラフで使用すること
 * - Cost は比較可能であること
 * - Sum は加算可能であること
 *
 * 使用例:
 * @code
 * std::vector<kwm_t::graph::Edge<int>> edges;
 *
 * edges.push_back({0, 1, 3});
 * edges.push_back({1, 2, 5});
 *
 * auto mst = kwm_t::graph::kruskal<int>(3, edges);
 * @endcode
 *
 * verified:
 * https://atcoder.jp/contests/awc0064/submissions/75580701
 */
template<class Cost, class Sum = long long>
MinimumSpanningTree<Cost, Sum> kruskal(int n, std::vector<Edge<Cost>> edges) {
	std::sort(edges.begin(), edges.end(),
		[](const Edge<Cost>& l, const Edge<Cost>& r) {return l.cost < r.cost; }
	);

	atcoder::dsu uf(n);

	Sum total = Sum();

	std::vector<Edge<Cost>> used_edges;

	for (const Edge<Cost>& e : edges) {
		if (uf.same(e.from, e.to)) continue;

		uf.merge(e.from, e.to);
		total += static_cast<Sum>(e.cost);
		used_edges.push_back(e);
	}

	const bool connected = ((int)used_edges.size() == n - 1);

	return { total, used_edges, connected };
}

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_KRUSKAL_HPP