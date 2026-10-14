/**
 * @file bipartite_vertex_cover.hpp
 * @brief 二部グラフの最小点被覆・最大独立集合
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/acl_wrapper/bipartite_vertex_cover.hpp
 */
#ifndef KWM_T_ACL_WRAPPER_BIPARTITE_VERTEX_COVER_HPP
#define KWM_T_ACL_WRAPPER_BIPARTITE_VERTEX_COVER_HPP

#include <queue>
#include <vector>
#include <atcoder/maxflow>

namespace kwm_t::acl_wrapper {

namespace detail {

template <class Cap>
std::vector<bool> bipartite_reachable(
	int L,
	int R,
	int s,
	const atcoder::mf_graph<Cap>& g
) {
	const int N = L + R + 2;

	std::vector<std::vector<int>> residual(N);

	for (const auto& e : g.edges()) {
		if (e.cap > e.flow) {
			residual[e.from].push_back(e.to);
		}
		if (e.flow > 0) {
			residual[e.to].push_back(e.from);
		}
	}

	std::vector<bool> vis(N, false);

	std::queue<int> q;
	q.push(s);
	vis[s] = true;

	while (!q.empty()) {
		int v = q.front();
		q.pop();

		for (int nv : residual[v]) {
			if (vis[nv]) continue;
			vis[nv] = true;
			q.push(nv);
		}
	}

	return vis;
}

} // namespace detail

/**
 * @brief 二部グラフの最小点被覆を復元
 *
 * flow(s, t) 実行後に呼ぶこと。
 *
 * 頂点番号:
 *   左側: [0, L)
 *   右側: [L, L + R)
 *
 * 計算量:
 *   O(V + E)
 */
template <class Cap>
std::vector<int> minimum_vertex_cover(
	int L,
	int R,
	int s,
	const atcoder::mf_graph<Cap>& g
) {
	auto vis = detail::bipartite_reachable(L, R, s, g);

	std::vector<int> res;

	for (int i = 0; i < L; i++) {
		if (!vis[i]) res.push_back(i);
	}

	for (int i = 0; i < R; i++) {
		if (vis[L + i]) res.push_back(L + i);
	}

	return res;
}

/**
 * @brief 二部グラフの最大独立集合を復元
 *
 * flow(s, t) 実行後に呼ぶこと。
 *
 * 頂点番号:
 *   左側: [0, L)
 *   右側: [L, L + R)
 *
 * 計算量:
 *   O(V + E)
 */
template <class Cap>
std::vector<int> maximum_independent_set(
	int L,
	int R,
	int s,
	const atcoder::mf_graph<Cap>& g
) {
	auto vis = detail::bipartite_reachable(L, R, s, g);

	std::vector<int> res;

	for (int i = 0; i < L; i++) {
		if (vis[i]) res.push_back(i);
	}

	for (int i = 0; i < R; i++) {
		if (!vis[L + i]) res.push_back(L + i);
	}

	return res;
}

} // namespace kwm_t::acl_wrapper

#endif // KWM_T_ACL_WRAPPER_BIPARTITE_VERTEX_COVER_HPP