/**
 * @file functional_graph.hpp
 * @brief Functional Graph
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/functional_graph.hpp
 */
#ifndef KWM_T_GRAPH_FUNCTIONAL_GRAPH_HPP
#define KWM_T_GRAPH_FUNCTIONAL_GRAPH_HPP

#include <vector>
#include <queue>
#include <algorithm>
#include <cstddef>

/**
 * @brief Functional Graph（各頂点の出次数1）
 *
 * vector<int> to を受け取り、コンストラクタで全構築する immutable 設計。
 *
 * 機能:
 *  - 連結成分（= 同じcycleに流れ込む）
 *  - cycleサイズ
 *  - cycle内位置
 *  - cycleまでの距離
 *  - k回遷移 (doubling)
 *
 * 典型用途:
 *  - k回後の遷移先
 *  - cycle検出
 *  - ループに入るまでの距離
 *
 * 計算量:
 *  - 構築: O(N log N)
 *  - クエリ: O(log N)
 *
 * @param to 各頂点の遷移先 (0-indexed)
 *
 * 制約 / 注意:
 *  - 0 <= to[i] < N
 *  - 各頂点の出次数は必ず1
 *
 * 使用例:
 *  vector<int> to = {...};
 *  FunctionalGraph fg(to);
 *  int v = fg.jump(0, 10);
 *
 * verified:
 *  https://atcoder.jp/contests/awc0030/submissions/74245105
 */
namespace kwm_t::graph {

	class FunctionalGraph {
	private:
		int n;
		std::vector<int> to;
		std::vector<std::vector<int>> rev;

		std::vector<int> comp;       // 連結成分ID
		std::vector<int> depth;      // cycleまでの距離
		std::vector<int> pos;        // cycle内位置
		std::vector<int> cycleSize;  // 各cycleのサイズ

		static constexpr int LOG = 30;
		std::vector<std::vector<int>> up;

		void build_rev() {
			rev.assign(n, {});
			for (int i = 0; i < n; ++i) {
				rev[to[i]].push_back(i);
			}
		}

		void build() {
			comp.assign(n, -1);
			depth.assign(n, -1);
			pos.assign(n, -1);
			cycleSize.clear();

			std::vector<int> state(n, 0); // 0:未訪問,1:探索中,2:確定
			int cid = 0;

			for (int i = 0; i < n; ++i) {
				if (state[i]) continue;

				int v = i;
				std::vector<int> path;

				while (state[v] == 0) {
					state[v] = 1;
					path.push_back(v);
					v = to[v];
				}

				if (state[v] == 1) {
					// cycle検出
					std::vector<int> cyc;
					for (int j = (int)path.size() - 1; j >= 0; --j) {
						cyc.push_back(path[j]);
						if (path[j] == v) break;
					}
					std::reverse(cyc.begin(), cyc.end());

					int sz = cyc.size();
					cycleSize.push_back(sz);

					for (int j = 0; j < sz; ++j) {
						int x = cyc[j];
						comp[x] = cid;
						depth[x] = 0;
						pos[x] = j;
					}

					// 木部分をBFS
					std::queue<int> q;
					for (auto x : cyc) q.push(x);

					while (!q.empty()) {
						int cur = q.front(); q.pop();
						for (auto nx : rev[cur]) {
							if (comp[nx] != -1) continue;
							comp[nx] = cid;
							depth[nx] = depth[cur] + 1;
							q.push(nx);
						}
					}

					cid++;
				}

				for (auto x : path) state[x] = 2;
			}

			// doubling
			up.assign(LOG, std::vector<int>(n));
			for (int i = 0; i < n; ++i) up[0][i] = to[i];
			for (int k = 1; k < LOG; ++k) {
				for (int i = 0; i < n; ++i) {
					up[k][i] = up[k - 1][up[k - 1][i]];
				}
			}
		}

	public:
		FunctionalGraph(const std::vector<int>& to)
			: n((int)to.size()), to(to) {
			build_rev();
			build();
		}

		// k回遷移
		int jump(int v, long long k) const {
			for (int i = 0; i < LOG; ++i) {
				if (k & (1LL << i)) {
					v = up[i][v];
				}
			}
			return v;
		}

		// 同じ連結成分
		bool same_component(int a, int b) const {
			return comp[a] == comp[b];
		}

		// cycleサイズ
		int get_cycle_size(int v) const {
			return cycleSize[comp[v]];
		}

		// cycleまでの距離
		int dist_to_cycle(int v) const {
			return depth[v];
		}

		// cycle上か
		bool on_cycle(int v) const {
			return depth[v] == 0;
		}

		// cycle内位置（cycle外は -1）
		int cycle_pos(int v) const {
			return pos[v];
		}
	};

}

#endif // KWM_T_GRAPH_FUNCTIONAL_GRAPH_HPP