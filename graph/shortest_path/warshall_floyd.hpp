#ifndef GRAPH_SHORTEST_PATH_WARSHALL_FLOYD_HPP
#define GRAPH_SHORTEST_PATH_WARSHALL_FLOYD_HPP

#include <vector>
#include <algorithm>

/**
 * @brief ワーシャルフロイド法（全点対最短経路）および辺追加による更新
 *
 * グラフの全頂点対間の最短距離を求めるアルゴリズム。
 * また、1本の辺を追加した際の距離更新（O(N^2)）も提供する。
 *
 * 典型用途:
 *   - 全点対最短距離 (APSP)
 *   - 小規模グラフ (N ≦ 400 程度)
 *   - 辺追加クエリ付き問題（差分更新）
 *
 * 計算量:
 *   - warshall_floyd: O(N^3)
 *   - warshall_floyd_update: O(N^2)
 *
 * @tparam T 距離型（int, long long, double など）
 *
 * @param d 距離行列（破壊的更新）
 * @param inf 到達不能を表す値
 * @param u, v 辺の端点
 * @param w 辺の重み
 * @param twoway 無向辺なら true
 *
 * 制約 / 注意:
 *   - d は事前に初期化されている必要がある
 *   - inf は十分大きい値であること
 *   - warshall_floyd_update は事前に warshall_floyd 実行後に使用する
 *   - オーバーフロー回避のため inf は余裕を持たせること
 *
 * 使用例:
 *   std::vector<std::vector<long long>> d(n, std::vector<long long>(n, INF));
 *   // 辺をセット
 *   warshall_floyd(d, INF);
 *   auto diff = warshall_floyd_update(d, INF, u, v, w, true);
 *
 * verified:
 *   https://atcoder.jp/contests/awc0029/submissions/74223310
 *   https://atcoder.jp/contests/abc416/submissions/74223415
 */
namespace graph::shortest_path {

	template<typename T>
	void warshall_floyd(std::vector<std::vector<T>> &d, T inf) {
		int n = (int)d.size();
		for (int k = 0; k < n; ++k) {
			for (int i = 0; i < n; ++i) {
				if (d[i][k] >= inf) continue;
				for (int j = 0; j < n; ++j) {
					if (d[k][j] >= inf) continue;
					// overflow guard
					if (d[i][k] > inf - d[k][j]) continue;
					T nd = d[i][k] + d[k][j];
					if (nd < d[i][j]) d[i][j] = nd;
				}
			}
		}
	}

	template<typename T>
	T warshall_floyd_update(std::vector<std::vector<T>> &d, T inf, int u, int v, T w, bool twoway = false) {
		int n = (int)d.size();
		if (d[u][v] <= w && (!twoway || d[v][u] <= w)) return T();

		T diff = T();
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {

				T best = d[i][j];

				// i -> u -> v -> j
				if (d[i][u] < inf && d[v][j] < inf) {
					best = std::min(best, (T)(d[i][u] + w + d[v][j]));
				}

				// i -> v -> u -> j
				if (twoway && d[i][v] < inf && d[u][j] < inf) {
					best = std::min(best, (T)(d[i][v] + w + d[u][j]));
				}

				if (d[i][j] <= best) continue;
				if (d[i][j] < inf) diff -= d[i][j];
				d[i][j] = best;
				diff += d[i][j];
			}
		}
		return diff;
	}

} // namespace graph::shortest_path

#endif // GRAPH_SHORTEST_PATH_WARSHALL_FLOYD_HPP