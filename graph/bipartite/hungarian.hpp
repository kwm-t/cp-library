#ifndef KWM_T_GRAPH_BIPARTITE_HUNGARIAN_HPP
#define KWM_T_GRAPH_BIPARTITE_HUNGARIAN_HPP

#include <vector>

/**
 * @brief 二部グラフの最小 / 最大重み完全マッチング（Hungarian 法）
 *
 * 左側の全頂点を、異なる右側の頂点に割り当てる。
 * 辺が存在しない組み合わせは割り当てに使用しない。
 *
 * Maximize = false: 最小重み完全マッチング
 * Maximize = true : 最大重み完全マッチング
 *
 * 計算量: O(n^2 * m) (n <= m)
 *         O(n^3)      (n == m)
 *
 * 完全マッチングが存在しない場合、size = 0。
 * 存在する場合、size = n。
 *
 * Cost は加減算と比較、および 0 からの構築が可能な型を想定する。
 * inf には、コストや計算途中の値より十分大きな値を指定する。
 *
 * Verified:
 * https://atcoder.jp/contests/arc232/submissions/79909961
 */

namespace kwm_t::graph::bipartite {

template <typename Cost>
struct HungarianEdge {
	int to;
	Cost cost;
};

template <typename Cost>
struct HungarianResult {
	// 完全マッチングが存在する場合は n、存在しない場合は 0
	int size = 0;

	// 最適値。size == n の場合のみ有効
	Cost value = Cost(0);

	// match_left[i] : 左頂点 i の割り当て先
	// match_right[j]: 右頂点 j の割り当て元
	// 完全マッチングが存在しない場合は空
	std::vector<int> match_left;
	std::vector<int> match_right;
};

template <typename Cost, bool Maximize = false>
HungarianResult<Cost> hungarian(
	int n,
	int m,
	const std::vector<std::vector<HungarianEdge<Cost>>>& g,
	Cost inf
) {
	using Edge = HungarianEdge<Cost>;
	using Result = HungarianResult<Cost>;

	Result res;
	if (n > m) return res;

	// Hungarian 法は n <= m の最小化問題として解く。
	// 最大化の場合は重みの符号を反転する。
	std::vector<std::vector<Cost>> cost(
		n, std::vector<Cost>(m, inf)
	);
	std::vector<std::vector<bool>> exists(
		n, std::vector<bool>(m, false)
	);

	for (int i = 0; i < n; ++i) {
		for (const Edge& e : g[i]) {
			int j = e.to;
			Cost c = Maximize ? -e.cost : e.cost;

			if (!exists[i][j] || c < cost[i][j]) {
				exists[i][j] = true;
				cost[i][j] = c;
			}
		}
	}

	// 1-indexed の Hungarian 法
	std::vector<Cost> u(n + 1, Cost(0));
	std::vector<Cost> v(m + 1, Cost(0));
	std::vector<int> p(m + 1, 0);
	std::vector<int> way(m + 1, 0);

	for (int i = 1; i <= n; ++i) {
		p[0] = i;
		int j0 = 0;

		std::vector<Cost> minv(m + 1, inf);
		std::vector<bool> used(m + 1, false);

		do {
			used[j0] = true;
			int i0 = p[j0];
			Cost delta = inf;
			int j1 = -1;

			for (int j = 1; j <= m; ++j) {
				if (used[j] || !exists[i0 - 1][j - 1]) {
					continue;
				}

				Cost cur =
					cost[i0 - 1][j - 1] - u[i0] - v[j];

				if (cur < minv[j]) {
					minv[j] = cur;
					way[j] = j0;
				}
			}

			for (int j = 1; j <= m; ++j) {
				if (used[j]) continue;

				if (minv[j] < delta) {
					delta = minv[j];
					j1 = j;
				}
			}

			// 増加路が存在しない
			if (j1 == -1 || delta == inf) return res;

			for (int j = 0; j <= m; ++j) {
				if (used[j]) {
					u[p[j]] += delta;
					v[j] -= delta;
				}
				else if (minv[j] != inf) {
					minv[j] -= delta;
				}
			}

			j0 = j1;
		} while (p[j0] != 0);

		do {
			int j1 = way[j0];
			p[j0] = p[j1];
			j0 = j1;
		} while (j0 != 0);
	}

	res.match_left.assign(n, -1);
	res.match_right.assign(m, -1);

	for (int j = 1; j <= m; ++j) {
		if (p[j] == 0) continue;

		int i = p[j] - 1;
		int k = j - 1;

		// 存在しない辺を使用していないか確認
		if (!exists[i][k]) return Result{};

		res.match_left[i] = k;
		res.match_right[k] = i;
	}

	res.size = n;
	for (int i = 0; i < n; ++i) {
		res.value += cost[i][res.match_left[i]];
	}

	if constexpr (Maximize) {
		res.value = -res.value;
	}

	return res;
}

}  // namespace kwm_t::graph::bipartite

#endif